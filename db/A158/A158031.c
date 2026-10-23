/*
 * A158031  Sides of equilateral triangles which are filled exactly (no holes,
 *          no overlaps) by the digits used to write a subsequence of
 *          consecutive triangular numbers, starting with 0.
 * https://oeis.org/A158031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th side length L such that the * cumulative number
 *            of decimal digits of the triangular numbers * T(k)=k(k+1)/2
 *            (starting with k=0) satisfies * Σ_{k=0}^{t} digits(T(k)) =
 *            L(L+1)/2. * The sequence lists these side lengths in increasing
 *            order
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_len, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Fixed-size cache sufficient for n up to at least 10000 */
    enum { MAX_CAP = 20000 };
    static seq_t  cache[MAX_CAP];
    static size_t cache_len   = 0;   /* number of terms stored   */
    static idx_t  next_tri_k  = 0;   /* next triangular index to process */
    static seq_t  running_sum = 0;   /* Σ digits up to next_tri_k-1 */

    /* Populate cache until we have the requested term */
    while (cache_len < n)
    {
        /* Current triangular number T = k*(k+1)/2 */
        seq_t tri = (seq_t)next_tri_k * ((seq_t)next_tri_k + 1) / 2;

        /* Number of decimal digits in T */
        seq_t d = (seq_t)digit_len(tri);

        /* Update cumulative digit count */
        running_sum += d;

        /* Check whether running_sum itself is triangular */
        seq_t disc  = running_sum * 8 + 1;               /* 8*S+1 */
        seq_t root  = floor_sqrt(disc);                /* integer sqrt */
        if (root * root == disc)
        {
            /* side length L satisfying L(L+1)/2 = running_sum */
            seq_t L = (root - 1) / 2;

            /* Store the new side length if we haven't reached the cache capacity */
            if (cache_len < MAX_CAP)
            {
                cache[cache_len++] = L;
            }
        }

        /* Advance to next triangular index */
        ++next_tri_k;
    }

    return cache[n - 1];
}
