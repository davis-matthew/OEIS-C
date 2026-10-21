/*
 * A231655  Triangle T(n, k) read by rows giving number of non-equivalent ways
 *          to choose k points in an equilateral triangle grid of side n.
 * https://oeis.org/A231655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 101, u64 n <= 173,
 *            u128 n <= 173, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A231655: OEIS triangle T(n,k) – number of orbits of choosing k points
 *  in an equilateral triangle grid of side n under the dihedral group
 *  D6 (order 6).  The flattened sequence a(n) is obtained by reading
 *  the triangle row–by–row, starting with n=1, k=0.
 *
 *  For a fixed side s the grid contains N = s*(s+1)/2 points.
 *  Let the index n correspond to (s,k), where 0 ≤ k ≤ N.  Then
 *
 *      a(n) = (  C(N,k)                          // identity
 *              + 2 * Σ_{t} C(z,t) C(m1_rot, k-3t)  // 120°/240°
 *              + 3 * Σ_{t} C(m2_ref,t) C(m1_ref, k-2t) // reflections
 *            ) / 6
 *
 *  where
 *      m1_rot = (s-1)%3==0 ? 1 : 0,   z     = (N - m1_rot)/3
 *      m1_ref = floor((s-1)/2)+1,     m2_ref = (N - m1_ref)/2
 *
 *  The formula follows from Burnside's lemma.
 *
 *  An implementation is given below that uses the provided binom()
 *  routine to compute binomial coefficients in type seq_t.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the side s and the position k in its row. */
    idx_t cum = 0;
    idx_t s = 1;                /* side lengths start at 1 */

    while (true) {
        /* Number of points in a side‑s triangle grid. */
        idx_t N = (s * (s + 1)) / 2;

        /* Length of the current row = N + 1. */
        idx_t row_len = N + 1;

        if (n < cum + row_len) {
            idx_t k = n - cum;          /* k ranges from 0 … N */

            /* ---- Identity element ---- */
            seq_t identity = binom(N, k);

            /* ---- Rotations by 120° and 240° ---- */
            idx_t m1_rot = ((s - 1) % 3 == 0) ? 1 : 0;
            idx_t z_rot  = (N - m1_rot) / 3;   /* number of 3‑cycles */

            seq_t rot = 0;
            idx_t max_t_rot = (k / 3 < z_rot) ? (k / 3) : z_rot;
            for (idx_t t = 0; t <= max_t_rot; ++t) {
                idx_t rest = k - 3 * t;
                if (rest > m1_rot) continue;
                rot += binom(z_rot, t) * binom(m1_rot, rest);
            }

            /* ---- Reflections (three of them) ---- */
            idx_t m1_ref = (s - 1) / 2 + 1;       /* fixed points */
            idx_t m2_ref = (N - m1_ref) / 2;      /* number of 2‑cycles */

            seq_t ref = 0;
            idx_t max_t_ref = (k / 2 < m2_ref) ? (k / 2) : m2_ref;
            for (idx_t t = 0; t <= max_t_ref; ++t) {
                idx_t rest = k - 2 * t;
                if (rest > m1_ref) continue;
                ref += binom(m2_ref, t) * binom(m1_ref, rest);
            }

            /* ---- Burnside's lemma --- */
            seq_t numerator = identity + 2 * rot + 3 * ref;
            return numerator / 6;                 /* integer division */
        }

        cum += row_len;
        ++s;
    }
}
