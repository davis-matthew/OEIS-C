/*
 * A181403  Total number of n-digit numbers requiring 7 positive cubes in
 *          their representation as sum of cubes.
 * https://oeis.org/A181403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The numbers a(n) count the n‑digit natural numbers whose minimal
 * representation as a sum of positive cubes uses exactly seven cubes.
 * They are given by a(n)=A181402(n)-A181402(n-1), where
 * A181402(n) counts all n‑digit numbers representable with at most
 * seven positive cubes.  For n≥5 all such numbers are representable
 * with at most six cubes, so the sequence is zero from n=5 onward. */

/* seq_t and idx_t are defined by the build. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    /* No value is defined for n<1 in the OEIS specification. */
    if (n < 1u) return (seq_t)0;
    /* For 5 or more digits the count is always zero. */
    if (n > 4u) return (seq_t)0;

    /* ------------------------------------------------------------
     * Pre‑compute minimal numbers of cubes for all integers 0..9999.
     * ------------------------------------------------------------ */
    static bool initialized = false;
    /* store minimal cube counts up to 7 (8‑bit is enough) */
    static unsigned char mincubes[10000];

    if (!initialized) {
        const unsigned char INF = 255;  /* greater than any needed value */
        size_t i;

        /* Initialise with "infinity". */
        for (i = 0; i < 10000; ++i) mincubes[i] = INF;
        mincubes[0] = 0;

        /* List of all cubes not exceeding 9999. */
        const idx_t cubes_vals[] = {
            1, 8, 27, 64, 125, 216, 343, 512,
            729, 1000, 1331, 1728, 2197, 2744, 3375,
            4096, 4913, 5832, 6859, 8000, 9261
        };
        const size_t cube_cnt = sizeof(cubes_vals) / sizeof(cubes_vals[0]);

        for (i = 1; i < 10000; ++i) {
            unsigned char best = INF;
            size_t j;
            for (j = 0; j < cube_cnt; ++j) {
                idx_t c = cubes_vals[j];
                if (c > i) break;
                unsigned char v = mincubes[i - (size_t)c] + 1;
                if (v < best) best = v;
            }
            mincubes[i] = best;
        }
        initialized = true;
    }

    /* ------------------------------------------------------------
     * Count the numbers in the n‑digit range that require exactly
     * seven cubes.
     * ------------------------------------------------------------ */
    /* Compute the lower and upper bounds of n‑digit numbers. */
    idx_t low  = 1;
    idx_t exp  = n - 1;
    while (exp--) low *= 10u;             /* low = 10^(n-1) */
    idx_t high = low * 10u - 1u;           /* high = 10^n - 1 */

    /* Count members of the range with mincubes value 7. */
    size_t cnt = 0;
    idx_t x;
    for (x = low; x <= high; ++x) {
        if (mincubes[(size_t)x] == 7u) ++cnt;
    }
    return (seq_t)cnt;
}
