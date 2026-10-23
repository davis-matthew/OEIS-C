/*
 * A123173  Least semiprime s for which the Mertens function M(s) = n.
 * https://oeis.org/A123173
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime s with the Mertens function M(s) = n
 *            (offset = -3). The function a(idx_t n) returns that least
 *            semiprime for every n >= -3
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- global tables -------------------------------------------------------*/
#define OFFSET (-3)

static uint32_t *spf   = NULL;   /* smallest prime factor for each n */
static int8_t   *mu    = NULL;   /* mobius function values */
static uint8_t  *ispri = NULL;   /* is prime flag */
static uint32_t *primes = NULL;  /* prime list */
static size_t   primes_cnt = 0;
static size_t   sieve_lim  = 0;

/* answer cache: ans[i] = least semiprime with Mertens = (i + OFFSET)   */
static seq_t *ans = NULL;
static bool *ans_set = NULL;
static size_t ans_cap = 0;

/* current sweep state */
static seq_t curr_M = 0;     /* current value of M(s) */
static idx_t  curr_s = 0;   /* last processed integer */

/*--- helper: dynamic array for ans -------------------------------------*/
static void grow_ans(size_t req)
{
    if (req < ans_cap) return;
    size_t new_cap = ans_cap ? ans_cap : 16;
    while (new_cap <= req) new_cap <<= 1;
    seq_t  *p   = (seq_t  *)realloc(ans,    new_cap * sizeof(seq_t));
    bool   *ps  = (bool   *)realloc(ans_set, new_cap * sizeof(bool));
    if (!p || !ps) exit(1);
    /* initialise new slots */
    for (size_t i = ans_cap; i < new_cap; ++i) {
        p[i] = 0;
        ps[i] = false;
    }
    ans = p;   ans_set = ps;   ans_cap = new_cap;
}

/*--- sieve --------------------------------------------------------------*/
static void init_sieve(size_t limit)
{
    /* free old data */
    free(spf);   free(mu);    free(ispri);   free(primes);
    spf   = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    mu    = (int8_t*)calloc(limit + 1, sizeof(int8_t));
    ispri = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    primes = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf || !mu || !ispri || !primes) exit(1);

    mu[1] = 1;
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i]   = (uint32_t)i;
            ispri[i] = 1;
            mu[i]    = -1;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > limit) break;
            spf[v] = p;
            if (p == spf[i]) {
                mu[v] = 0;
                break;
            } else
                mu[v] = -mu[i];
        }
    }
    sieve_lim = limit;
}

static void ensure_sieve_for(idx_t n)
{
    if ((size_t)n > sieve_lim) {
        size_t new_lim = sieve_lim ? sieve_lim * 2 : 16;
        while (new_lim <= (size_t)n) new_lim <<= 1;
        init_sieve(new_lim);
    }
}

/*--- semiprime test -----------------------------------------------------*/
static bool is_semiprime(idx_t x)
{
    if (x < 4) return false;
    uint32_t p = spf[x];
    uint32_t q = (uint32_t)(x / p);
    return (q >= 2 && (q == p || ispri[q]));
}

/*--- main function ------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < OFFSET) return 0;   /* undefined below offset */

    /* compute the answer for all indices up to n if we have not seen it */
    size_t idx_needed = (size_t)(n - OFFSET);
    grow_ans(idx_needed);

    if (ans_set[idx_needed]) return ans[idx_needed];

    /* initialise sieve if necessary */
    if (!spf) init_sieve(2000000);

    /* continue scanning numbers until the requested index is filled */
    while (true) {
        ++curr_s;
        ensure_sieve_for(curr_s);
        /* add Mobius value */
        curr_M += (seq_t)mu[curr_s];

        if (is_semiprime(curr_s)) {
            /* current M value is curr_M, offset to index */
            idx_t target = (idx_t)(curr_M - OFFSET);
            if (target >= 0) {
                size_t pos = (size_t)target;
                grow_ans(pos);
                if (!ans_set[pos]) {
                    ans[pos] = curr_s;
                    ans_set[pos] = true;
                }
                if (pos == idx_needed) return ans[idx_needed];
            }
        }
    }
}
