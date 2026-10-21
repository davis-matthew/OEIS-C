/*
 * A221194  Simple continued fraction expansion of product {k >= 0} (1 - 2*(N
 *          - sqrt(N^2-1))^(4*k+3))/(1 - 2*(N - sqrt(N^2-1))^(4*k+1)) at N =
 *          4.
 * https://oeis.org/A221194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A221194: Simple continued fraction expansion of
 *   product_{k >= 0} (1 - 2*(4 - sqrt(15))^(4*k+3)) /
 *                   (1 - 2*(4 - sqrt(15))^(4*k+1))
 *   (N = 4).
 * Formula:
 *   a(4*n)   = 1
 *   a(4*n+1) = ((4+sqrt15)^(2*n+1) + (4-sqrt15)^(2*n+1)) / 2 - 2
 *   a(4*n-1) = (4+sqrt15)^(2*n) + (4-sqrt15)^(2*n) - 2
 * The integer sequence A_n = (4+sqrt15)^n + (4-sqrt15)^n satisfies
 *   A_0 = 2, A_1 = 8,
 *   A_n = 8*A_{n-1} - A_{n-2}.
 * The algorithm caches the A values to compute each term in O(1)
 * amortised time.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, abort */

static seq_t *A_cache = NULL;   /* cached values of A_n */
static size_t A_len   = 0;      /* number of entries in A_cache */

/* Ensure that A_cache contains values A_0 ... A_m (inclusive) */
static void ensure_A(size_t m)
{
    if (m < A_len)
        return;

    size_t old_len = A_len;
    seq_t *old_cache = A_cache;
    size_t new_len = m + 1;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_len];
#else
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) abort();
#endif

    /* copy old values */
    for (size_t i = 0; i < old_len; ++i)
        new_cache[i] = old_cache[i];

#ifdef __cplusplus
    delete[] old_cache;
#else
    free(old_cache);
#endif

    A_cache = new_cache;
    A_len   = new_len;

    /* initialise missing base cases if needed */
    if (old_len <= 0) {
        new_cache[0] = (seq_t)2;
        old_len = 1;
    }
    if (old_len <= 1) {
        new_cache[1] = (seq_t)8;
        old_len = 2;
    }

    /* compute remaining terms using recurrence A_n = 8*A_{n-1} - A_{n-2} */
    for (size_t i = old_len; i <= m; ++i) {
        /* seq_t multiplication and subtraction are defined for seq_t */
        new_cache[i] = (seq_t)8 * new_cache[i - 1] - new_cache[i - 2];
    }
}

seq_t a(idx_t n)
{
    /* even indices give 1 */
    if ((n & 1ULL) == 0) {
        return (seq_t)1;
    }

    /* odd indices: determine case by n mod 4 */
    if ((n & 3ULL) == 1ULL) {          /* n = 4*b + 1 */
        idx_t b = (n - 1) >> 2;        /* b = (n-1)/4 */
        size_t m = (size_t)(2 * b + 1);/* exponent for A */
        ensure_A(m);
        /* a(4*b+1) = A_{2*b+1} / 2 - 2 */
        seq_t val = A_cache[m] / (seq_t)2 - (seq_t)2;
        return val;
    } else {                           /* n = 4*b - 1 (i.e. n mod 4 == 3) */
        idx_t b = (n + 1) >> 2;        /* b = (n+1)/4 */
        size_t m = (size_t)(2 * b);     /* exponent for A */
        ensure_A(m);
        /* a(4*b-1) = A_{2*b} - 2 */
        seq_t val = A_cache[m] - (seq_t)2;
        return val;
    }
}
