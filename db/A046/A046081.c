/*
 * A046081  Number of integer-sided right triangles with n as a hypotenuse or
 *          leg.
 * https://oeis.org/A046081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A046081 Number of integer‑sided right triangles with n as a hypotenuse or leg.
   Implemented by generating all primitive Pythagorean triples with a side ≤ n,
   then counting all integer multiples of each primitive triple whose sides are ≤ n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Euclidean GCD for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    static seq_t *counts = NULL;      /* array of results for indices 0..max_alloc */
    static idx_t max_alloc = 0;       /* maximum index for which counts[] is valid */

    if (n == 0) return (seq_t)0;

    /* If requested n exceeds current cache, rebuild up to n */
    if (n > max_alloc) {
        /* Allocate new array */
        idx_t new_max = n;
        if (counts) free(counts);
        counts = (seq_t *)calloc((size_t)new_max + 1, sizeof(seq_t));
        if (!counts) return (seq_t)0;  /* allocation failure – unlikely for small n */

        /* Generate all primitive Pythagorean triples with at least one side ≤ new_max */
        /* Upper bound for m: for m > new_max/2 + 1, the smallest leg (2mn or m²−n²)
           is always > new_max, so no triple can contribute. */
        idx_t m_limit = new_max / 2 + 1;
        for (idx_t m = 2; m <= m_limit; ++m) {
            for (idx_t n1 = 1; n1 < m; ++n1) {
                /* m and n1 must be coprime and of opposite parity */
                if (((m - n1) & 1) == 0) continue;
                if (gcd_idx(m, n1) != 1) continue;

                idx_t a0 = m * m - n1 * n1;       /* odd leg */
                idx_t b0 = 2 * m * n1;           /* even leg */
                idx_t c0 = m * m + n1 * n1;       /* hypotenuse */

                /* If none of the sides of the primitive triple is ≤ new_max,
                   no scaled copy can contribute either. */
                if (a0 > new_max && b0 > new_max && c0 > new_max) continue;

                /* Scale the primitive triple by k */
                for (idx_t k = 1; ; ++k) {
                    idx_t a = a0 * k;
                    idx_t b = b0 * k;
                    idx_t c = c0 * k;
                    bool any = false;

                    if (a <= new_max) { counts[a]++; any = true; }
                    if (b <= new_max) { counts[b]++; any = true; }
                    if (c <= new_max) { counts[c]++; any = true; }

                    /* If no side of this scaled triple is ≤ new_max,
                       further multiples will also be too large. */
                    if (!any) break;
                }
            }
        }
        max_alloc = new_max;
    }

    return counts[n];
}
