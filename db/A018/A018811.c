/*
 * A018811  Number of lines through exactly 4 points of an n X n grid of
 *          points.
 * https://oeis.org/A018811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 (f(n,5) - 2 f(n,4) + f(n,3)) where f(n,k) = Σ( n -
 *            |k·x| )( n - |k·y| ) over all integer pairs (x,y) with
 *            0<|x|,|y|<n/k and gcd(|x|,|y|)=1, plus horizontal/vertical
 *            contributions for |x|=0 or |y|=0
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 737, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_int.h"

/* Euclidean algorithm for non‑negative integers. */

/* Compute f(n,k) as described. */
static seq_t f_val(idx_t n, int k) {
    if (n == 0) return (seq_t)0;

    /* maximum absolute x or y such that |k·x| < n. */
    int max_abs = (int)((n - 1) / (idx_t)k);

    seq_t sum = (seq_t)0;

    /* Horizontal and vertical lines (x=0 or y=0, |x|=|y|=1). */
    if (k < (int)n) {
        /* 4 lines: two for x=±1,y=0 and two for x=0,y=±1. */
        sum += (seq_t)4 * (seq_t)n * ((seq_t)n - (seq_t)k);
    }

    if (max_abs > 0) {
        /* Temporary storage for n - k·x values. */
        static int prod[4001];            /* size >= floor((10000-1)/3)+1 = 3334 */

        for (int x = 1; x <= max_abs; ++x) {
            prod[x] = (int)n - k * x;     /* positive because x <= max_abs */
        }

        for (int x = 1; x <= max_abs; ++x) {
            int px = prod[x];
            for (int y = 1; y <= max_abs; ++y) {
                if (gcd_int(x, y) != 1) continue;
                int py = prod[y];
                sum += (seq_t)4 * (seq_t)px * (seq_t)py;
            }
        }
    }

    return sum;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n) {
    /* The formula works for all n ≥ 0; intermediate values are
       zero for n < 4, so no special case handling is needed. */
    seq_t f3 = f_val(n, 3);
    seq_t f4 = f_val(n, 4);
    seq_t f5 = f_val(n, 5);

    seq_t numerator = f5 - (seq_t)2 * f4 + f3;
    return numerator / ((seq_t)2);
}
