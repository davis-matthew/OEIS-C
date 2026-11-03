/*
 * A033317  Smallest positive integer y satisfying the Pell equation x^2 -
 *          D*y^2 = 1 for nonsquare D.
 * https://oeis.org/A033317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt((A033313(n)^2 - 1)/A000037(n)) * A033313(n) is the
 *            minimal x solving x² – D·y² = 1 for the nth * non‑square D =
 *            A000037(n); this function computes the corresponding * minimal
 *            y. * * The implementation finds the D by scanning forward and
 *            then uses the * standard continued‑fraction algo...
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 98, u64 n <= 377, u128 n <= 989,
 *            big any n
 * Validated: standard, pass, 87 terms through n = 9224 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0 */

    /* ------- find the nth non‑square integer D ------- */
    seq_t cand = 2;
    idx_t  count = 0;
    seq_t D = 0;

    while (count < n) {
        seq_t r = floor_sqrt(cand);
        if (r * r != cand) {             /* cand is not a perfect square */
            count++;
            if (count == n) {
                D = cand;
                break;
            }
        }
        cand = cand + 1;
    }

    /* ------- compute minimal solution (x,y) of x² – D·y² = 1 -------
     * Using the continued‑fraction expansion of √D. */
    seq_t a0 = floor_sqrt(D);
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a  = a0;

    seq_t h_prev2 = 0, h_prev1 = 1;
    seq_t k_prev2 = 1, k_prev1 = 0;

    while (true) {
        seq_t h = a * h_prev1 + h_prev2;
        seq_t k = a * k_prev1 + k_prev2;

        /* Check whether this convergent satisfies Pell's equation. */
        if (h * h - D * k * k == 1)
            return k;                      /* k is the minimal y */

        /* Advance the continued‑fraction terms. */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        h_prev2 = h_prev1;  h_prev1 = h;
        k_prev2 = k_prev1;  k_prev1 = k;
    }
}
