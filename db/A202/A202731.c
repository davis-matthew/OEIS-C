/*
 * A202731  Number of n X 4 nonnegative integer arrays each row and column an
 *          ascent sequence (interior element no greater than one plus
 *          up-steps preceding it) and the lower right element equal to
 *          min(n,4)-1.
 * https://oeis.org/A202731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 492*a(n-2) + 3418*a(n-3) - 13215*a(n-4) +
 *            28650*a(n-5) - 32500*a(n-6) + 15000*a(n-7) for n > 10 Initial
 *            values: a(1)=1, a(2)=4, a(3)=14, a(4)=12, a(5)=462, a(6)=10296,
 *            a(7)=184848, a(8)=3010632, a(9)=46867734, a(10)=714602604
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[n] = a(n)  */
static idx_t cached_n = 0;      /* number of precomputed terms */

/* Ensure cache holds values up to n (inclusive).  Returns 0 on success,
   non‑zero on allocation failure.  */
static int ensure_cache(idx_t n)
{
    if (n <= cached_n)
        return 0;

    /* Allocate or enlarge cache */
    idx_t new_len = n + 1;          /* we keep index 0 unused */
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache)
        return -1;                 /* allocation failed */

    /* Initialise missing entries with recurrence during expansion */
    static const seq_t base[11] = {
        0ULL,                     /* dummy for index 0 */
        (seq_t)1ULL, (seq_t)4ULL, (seq_t)14ULL, (seq_t)12ULL,
        (seq_t)462ULL, (seq_t)10296ULL, (seq_t)184848ULL,
        (seq_t)3010632ULL, (seq_t)46867734ULL, (seq_t)714602604ULL
    };

    if (cached_n == 0) {
        /* first time: copy first 10 base elements that fit */
        for (idx_t i = 1; i <= 10 && i < new_len; ++i)
            cache[i] = base[i];
        cached_n = (new_len > 10 ? 10 : new_len - 1);
    }

    /* compute remaining terms via recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = base[i];
            continue;
        }
        /* recurrence:
           a(i) = 36*a(i-1) - 492*a(i-2) + 3418*a(i-3) - 13215*a(i-4)
                  + 28650*a(i-5) - 32500*a(i-6) + 15000*a(i-7)
        */
        seq_t larger = (seq_t)0;
        seq_t smaller = (seq_t)0;

        larger += cache[i-1] * (seq_t)36ULL;
        larger += cache[i-3] * (seq_t)3418ULL;
        larger += cache[i-5] * (seq_t)28650ULL;
        larger += cache[i-7] * (seq_t)15000ULL;

        smaller += cache[i-2] * (seq_t)492ULL;
        smaller += cache[i-4] * (seq_t)13215ULL;
        smaller += cache[i-6] * (seq_t)32500ULL;

        /* larger >= smaller for all n, so subtraction is safe */
        cache[i] = larger - smaller;
    }

    cached_n = n;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (ensure_cache(n) != 0)
        return (seq_t)0;          /* allocation failure – unlikely in tests */
    return cache[n];
}
