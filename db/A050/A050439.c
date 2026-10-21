/*
 * A050439  Fifth-order composites.
 * https://oeis.org/A050439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(C(C(C(C(n)))))
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the fifth‑order composite of the index n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the sieve and composite list */
static uint8_t      *is_prime    = NULL;
static uint64_t     *composites  = NULL;
static size_t        composites_len = 0;
static size_t        sieve_limit    = 0;

/* recompute the sieve and the composite list up to 'limit' */
static void recompute_sieve(size_t limit)
{
    uint8_t *prime  = (uint8_t *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) prime[i] = 1;
    if (limit >= 0) { prime[0] = 0; if (limit >= 1) prime[1] = 0; }

    for (size_t p = 2; p * p <= limit; ++p)
        if (prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                prime[m] = 0;

    /* collect composites */
    size_t cnt = 0;
    for (size_t i = 4; i <= limit; ++i)
        if (!prime[i]) ++cnt;

    uint64_t *comp = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 4; i <= limit; ++i)
        if (!prime[i]) comp[idx++] = (uint64_t)i;

    free(is_prime);
    free(composites);
    is_prime   = prime;
    composites = comp;
    composites_len = cnt;
    sieve_limit = limit;
}

/* ensure that composites list has at least 'need' entries */
static void ensure_composites_upto_index(idx_t need)
{
    while (composites_len < need) {
        size_t new_limit = (sieve_limit == 0) ? 20 : sieve_limit * 2;
        if (new_limit < (size_t)need * 2) new_limit = (size_t)need * 2;
        recompute_sieve(new_limit);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* n is always >=1 by definition */
    idx_t cur = n;
    for (int i = 0; i < 5; ++i) {
        ensure_composites_upto_index(cur);
        cur = composites[cur - 1];   /* composites are 0‑based, C(1)=4 */
    }
    return (seq_t)cur;
}
