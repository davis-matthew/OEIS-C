/*
 * A022725  Expansion of 1/((1-x)*(1-5*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A022725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1
 *   a(1) = 26
 *   a(2) = 455
 *   a(3) = 6760
 *   a(n) = 26*a(n-1) - 221*a(n-2) + 676*a(n-3) - 480*a(n-4)   (n >= 4)
 */
#include <stddef.h>   /* for size_t */

#ifndef MAX_N
/* For safety, allow larger n by dynamic allocation if needed */
#define MAX_N 10000
#endif

static seq_t cache[MAX_N + 1];
static size_t cache_len = 0;   /* number of entries currently computed (>=4 once initialized) */

seq_t a(idx_t n)
{
    /* Limit deviation from expected maximal n; extend array if necessary */
    if ((idx_t)MAX_N < n) {
        /* If n exceeds the predetermined limit, compute the term on demand
           without caching everything. This path assumes that such large n
           will not be called frequently. */
        idx_t i = 4;
        seq_t x0 = cache[0];
        seq_t x1 = cache[1];
        seq_t x2 = cache[2];
        seq_t x3 = cache[3];
        /* Compute up to n */
        for (i = 4; i <= n; ++i) {
            seq_t t1 = ((seq_t)26) * x3;
            seq_t t2 = ((seq_t)676) * x1;
            seq_t t3 = ((seq_t)221) * x2;
            seq_t t4 = ((seq_t)480) * x0;
            seq_t next = t1 + t2;
            next -= t3;
            next -= t4;
            x0 = x1;
            x1 = x2;
            x2 = x3;
            x3 = next;
        }
        return x3;
    }

    /* Normal path: use static cache up to MAX_N. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)455;
        cache[3] = (seq_t)6760;
        cache_len = 4;
    }

    if ((size_t)n >= cache_len) {
        size_t i;
        /* Compute missing terms up to n */
        for (i = cache_len; i <= (size_t)n; ++i) {
            seq_t t1 = ((seq_t)26) * cache[i-1];
            seq_t t2 = ((seq_t)676) * cache[i-3];
            seq_t t3 = ((seq_t)221) * cache[i-2];
            seq_t t4 = ((seq_t)480) * cache[i-4];
            seq_t val = t1 + t2;   /* term1 + term2 */
            val -= t3;             /* subtract term3 */
            val -= t4;             /* subtract term4 */
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
