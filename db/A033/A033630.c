/*
 * A033630  Number of partitions of n into distinct divisors of n.
 * https://oeis.org/A033630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct divisors of n.
 *            The function a(n) returns that count for any n ≥ 0 using a
 *            subset‑sum DP. It works for all supported seq_t types and is
 *            fast for n up to ≈10000
 * Range:     u8 n <= 119, u16 n <= 719, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve up to 10000 (enough for all required factorizations).  */
static uint64_t *primes   = NULL;
static size_t    primes_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL) return;          /* already built */

    const idx_t limit = 10000;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) return;           /* out of memory – let caller fail */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    /* Count primes first */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++primes_cnt;

    /* Allocate and fill the array */
    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        return;
    }
    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            primes[pidx++] = i;

    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*  Generate all positive divisors of n (including 1 and n).           */
static void get_divisors(idx_t n, idx_t *out_divs, size_t *cnt_out)
{
    if (n == 0) {          /* undefined – caller handles it */
        *cnt_out = 0;
        return;
    }

    init_primes();

    /* Factor n */
    idx_t tmp = n;
    uint64_t pf[16];      /* enough for n <= 10^7 */
    idx_t  exp[16];
    size_t pf_cnt = 0;

    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pf_cnt] = p;
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {
        pf[pf_cnt] = tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Generate divisors by multiplying prime powers */
    out_divs[0] = 1;
    size_t div_cnt = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        idx_t   emax = exp[i];
        size_t old_cnt = div_cnt;

        /* powers of p */
        uint64_t pk = 1;
        for (idx_t e = 1; e <= emax; ++e) {
            pk *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                out_divs[div_cnt++] = out_divs[j] * pk;
            }
        }
    }

    *cnt_out = div_cnt;
}

/* ------------------------------------------------------------------ */
/*  Main routine: number of partitions of n into distinct divisors.    */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* by definition */

    /* Obtain the divisor list */
    idx_t divs[256];               /* enough for n <= 10000 (max 25 divisors) */
    size_t div_cnt;
    get_divisors(n, divs, &div_cnt);

    /* DP array for subset–sum counts; allocate on heap to avoid stack limits. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;             /* allocation failure → zero count */

    dp[0] = 1;                     /* empty sum */

    for (size_t di = 0; di < div_cnt; ++di) {
        idx_t d = divs[di];
        for (idx_t s = n; s >= d; --s)
            dp[s] += dp[s - d];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
