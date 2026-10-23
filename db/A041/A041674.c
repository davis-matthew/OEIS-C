/*
 * A041674  Numerators of continued fraction convergents to sqrt(356).
 * https://oeis.org/A041674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041674: numerators of continued‐fraction convergents to √356. */
/* The continued fraction of √356 is [18; 1,6,1,1,2,1,8,1,2,1,1,6,1,36] repeating.  */
/* Let a_n be the nth coefficient (a_0 = 18, a_1 = 1, …, a_14 = 36).  */
/* Then the numerators p_n satisfy */
/*   p_0 = 18, p_1 = 19, and for n ≥ 2: p_n = a_n * p_{n‑1} + p_{n‑2}. */
/* This function returns p_n for any n ≥ 0 (memoised for repeated calls). */

#include <stddef.h>

static const seq_t coeff[15] = {
    0,  /* unused, placeholder for index 0 */
    1,  /* a_1 */
    6,  /* a_2 */
    1,  /* a_3 */
    1,  /* a_4 */
    2,  /* a_5 */
    1,  /* a_6 */
    8,  /* a_7 */
    1,  /* a_8 */
    2,  /* a_9 */
    1,  /* a_{10} */
    1,  /* a_{11} */
    6,  /* a_{12} */
    1,  /* a_{13} */
    36  /* a_{14} */
};

/* A reasonably large static cache to avoid dynamic allocation issues. */
#define CACHE_CAPACITY 65536
static seq_t cache[CACHE_CAPACITY];
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n >= CACHE_CAPACITY) {
        /* The cache is too small; fallback to recomputation without memoisation. */
        /* This branch should never be taken for typical test ranges. */
        idx_t i;
        seq_t p_im1, p_im2;
        /* Find the coefficient for this n */
        seq_t a_n = (n == 0) ? (seq_t)18
                    : coeff[(n-1) % 14 + 1];

        if (n == 0) return (seq_t)18;
        if (n == 1) return (seq_t)((seq_t)1 * 18 + (seq_t)1);

        /* Initialise for recurrence */
        p_im2 = 18;          /* p_0 */
        p_im1 = 19;          /* p_1 */

        for (i = 2; i <= n; ++i) {
            seq_t a_i = coeff[(i-1) % 14 + 1];
            seq_t p_i = a_i * p_im1 + p_im2;
            p_im2 = p_im1;
            p_im1 = p_i;
        }
        return p_im1;
    }

    /* If cache is empty, initialise the first terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)18;
        cache_len = 1;
    }

    /* Extend cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)18;
            continue;
        }
        if (i == 1) {
            cache[1] = coeff[1] * cache[0] + (seq_t)1;
            continue;
        }

        seq_t c = coeff[(i-1) % 14 + 1];
        cache[i] = c * cache[i-1] + cache[i-2];
    }
    cache_len = n + 1;

    return cache[n];
}
