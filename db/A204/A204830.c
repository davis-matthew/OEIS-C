/*
 * A204830  Numbers k whose divisors can be partitioned into three disjoint
 *          sets whose sums are all sigma(k)/3.
 * https://oeis.org/A204830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A204830: numbers k whose set of positive divisors can be
 *            partitioned into three disjoint subsets with equal sum σ(k)/3.
 *            The function produces the n-th term (a(1) = 120, a(2) = 180, …)
 * Range:     u8 n <= 3, u16 n <= 1215, u32 n <= 8925, u64 n <= 8925,
 *            u128 n <= 8925, big any n
 * Helpers:   cmp_int_desc, subset_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/cmp_int_desc.h"
#include "../utils/subset_rec.h"

/* ----------  global data --------------------------------------------------- */

static uint32_t *spf = NULL;      /* smallest prime factor table */
static idx_t spf_limit = 0;       /* maximal number for which spf is built */

static seq_t *seq_vals = NULL;    /* generated sequence terms */
static size_t seq_len = 0;        /* how many terms are stored */
static size_t seq_cap = 0;        /* allocated capacity of seq_vals */

/* ----------  helper definitions -------------------------------------------*/

struct factor { uint32_t p; uint32_t e; };

/* ----------  linear sieve -----------------------------------------------*/
static void build_spf(idx_t limit)
{
    if (spf_limit >= limit) return;   /* already built */
    if (spf) free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) abort();
    spf_limit = limit;
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    size_t primes_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc((limit / 10 + 1) * sizeof(uint32_t));
    if (!primes) abort();

    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if ((idx_t)p * i > limit) break;
            spf[p * i] = p;
            if (i % p == 0) break;
        }
    }
    free(primes);
}

/* ----------  factorisation ----------------------------------------------*/
static void factorize_uint32(uint32_t n, struct factor *factors, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        factors[(*cnt)++] = (struct factor){ p, exp };
    }
}

/* ----------  compute sigma ----------------------------------------------*/
static uint64_t sigma_of(const struct factor *factors, size_t cnt)
{
    uint64_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = factors[i].p;
        uint32_t exp = factors[i].e;
        uint64_t term = 1;
        uint64_t power = 1;
        for (uint32_t j = 0; j < exp; ++j) {
            power *= p;
            term += power;
        }
        sigma *= term;
    }
    return sigma;
}

/* ----------  generate all divisors (excluding n) ------------------------*/
static void gen_divs_recurse(const struct factor *factors, size_t idx,
                             uint32_t cur, uint32_t orig_n,
                             uint32_t *divs, size_t *cnt)
{
    if (idx == 0 && cur == orig_n) {
        /* root call before recursion; skip */
    }
    if (idx == (size_t)factors[0].e) { /* placeholder, never reached */
    }

    /* Simplest: recursion over factors. We use two indexes: factor index & exponent progress. */
}
static void gen_divs_helper(const struct factor *factors, size_t fac_cnt,
                            size_t idx, uint32_t cur,
                            uint32_t orig_n,
                            uint32_t *divs, size_t *cnt)
{
    if (idx == fac_cnt) {
        if (cur != orig_n) {
            divs[(*cnt)++] = cur;
        }
        return;
    }
    uint32_t p = factors[idx].p;
    uint32_t exp = factors[idx].e;
    uint32_t val = 1;
    for (uint32_t e = 0; e <= exp; ++e) {
        gen_divs_helper(factors, fac_cnt, idx + 1, cur * val,
                        orig_n, divs, cnt);
        val *= p;
    }
}
static void generate_divisors(const struct factor *factors, size_t fac_cnt,
                              uint32_t orig_n, uint32_t *divs, size_t *cnt)
{
    *cnt = 0;
    gen_divs_helper(factors, fac_cnt, 0, 1, orig_n, divs, cnt);
}

static bool has_partition(uint32_t n)
{
    struct factor facs[32];
    size_t fac_cnt;
    factorize_uint32(n, facs, &fac_cnt);

    uint64_t sigma = sigma_of(facs, fac_cnt);
    if (sigma % 3 != 0) return false;
    if (sigma < 3ull * n) return false;           /* sigma/3 >= n required */

    int target = (int)(sigma / 3);

    /* generate all divisors except n */
    uint32_t divs_u32[256];
    size_t div_cnt;
    generate_divisors(facs, fac_cnt, n, divs_u32, &div_cnt);

    if (div_cnt == 0) return false;               /* only divisor is n itself */

    /* sort descending */
    int *divs = (int *)malloc(div_cnt * sizeof(int));
    if (!divs) abort();
    for (size_t i = 0; i < div_cnt; ++i) divs[i] = (int)divs_u32[i];
    qsort(divs, div_cnt, sizeof(int), cmp_int_desc);

    /* suffix sums */
    int *suff = (int *)malloc((div_cnt + 1) * sizeof(int));
    if (!suff) abort();
    suff[div_cnt] = 0;
    for (size_t i = div_cnt; i > 0; --i)
        suff[i - 1] = suff[i] + divs[i - 1];

    bool ok = subset_rec(divs, suff, 0, 0, target, div_cnt);

    free(divs);
    free(suff);
    return ok;
}

/* ----------  push to sequence -------------------------------------------*/
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!newp) abort();
        seq_vals = newp;
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* ----------  main sequence function --------------------------------------*/
#define INITIAL_SIEVE_LIMIT 10000000ull   /* 10 million, safe for 10k terms */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t candidate = 1;
    if (!initialized) {
        build_spf(INITIAL_SIEVE_LIMIT);
        initialized = true;
    }

    while (seq_len < n) {
        if (candidate > spf_limit) abort();  /* impossible in this setup */

        if (has_partition((uint32_t)candidate)) {
            seq_push((seq_t)candidate);
        }
        ++candidate;
    }
    return seq_vals[n - 1];
}
