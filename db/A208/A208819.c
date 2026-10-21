/*
 * A208819  Number of n-bead necklaces labeled with numbers -2..2 allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom, phi_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208819 – Necklaces with beads labeled -2..2, sum zero (allowing reversal).
 *
 *  We use Burnside’s lemma.  For a rotation of step g the cycle structure has
 *  d = gcd(n,g) cycles, each of length n/d.  The number of colourings fixed
 *  by such a rotation and having total weight zero equals the coefficient of
 *  y^0 in (y^-2+y^-1+1+y+y^2)^d, i.e. the number of length‑d sequences
 *  over {-2,-1,0,1,2} with sum 0.  A shift by +2 turns this into the
 *  number of solutions to
 *      x1+⋯+x_d = 2d ,   0≤xi≤4,
 *  which we compute via the inclusion–exclusion formula implemented in
 *  count_solutions().
 *
 *  For reflections we distinguish the parity of n.
 *
 *  • n odd: each reflection fixes one bead and pairs up the remaining
 *    (n‑1)/2 beads.  The fixed bead must be even, giving three cases:
 *    weight 0 (needs sum 0 in the pairs) or weight ±2 (needs sum ∓1).
 *
 *  • n even: there are two types of reflections.
 *      – type A: two beads are fixed, the rest are paired.
 *        The two fixed beads contribute a sum t∈{±4,±2,0},
 *        and the pairs must sum to –t/2.
 *        There are pairCnt[t] pairs of fixed beads with sum t.
 *      – type B: all beads are paired.
 *        The pairs must sum to 0.
 *
 *  The overall result is
 *      a(n) = (rot_sum + ref_sum) / (2n),
 *  where rot_sum aggregates over all rotations and ref_sum over all
 *  reflections.
 *
 *  Complexity:  O(τ(n)) for the rotation phase and constant work for the
 *  reflection phase.  For n ≤ 10 000 this is easily below the 2 s limit.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/phi_num.h"

/* ---------- number of solutions of 0 ≤ xi ≤ 4, sum = k ---------- */
static seq_t count_solutions(idx_t d, idx_t k)
{
    if (d == 0) return (k == 0) ? 1 : 0;
    if (k > 4 * d) return 0;

    seq_t total = 0;
    idx_t maxj = k / 5;
    if (maxj > d) maxj = d;

    for (idx_t j = 0; j <= maxj; ++j)
    {
        seq_t term =
            binom(d, j) *
            binom(k - 5 * j + d - 1, d - 1);
        if (j & 1)
            total -= term;
        else
            total += term;
    }
    return total;
}

/* ---------- pair counts for two beads (sum s) ----------
 *      [-4,-3,-2,-1,0,1,2,3,4]
 *  pairCnt[0] = 1   // (-2,-2)
 *  pairCnt[1] = 2   // (-2,-1),(-1,-2)
 *  pairCnt[2] = 3   // (-2,0),(-1,-1),(0,-2)
 *  pairCnt[3] = 4   // (-2,1),(-1,0),(0,-1),(1,-2)
 *  pairCnt[4] = 5   // (-2,2),(-1,1),(0,0),(1,-1),(2,-2)
 *  pairCnt[5] = 4   // (-1,2),(0,1),(1,0),(2,-1)
 *  pairCnt[6] = 3   // (0,2),(1,1),(2,0)
 *  pairCnt[7] = 2   // (1,2),(2,1)
 *  pairCnt[8] = 1   // (2,2)
 */
static const int8_t pairCnt[9] = {
    1, 2, 3, 4, 5, 4, 3, 2, 1
};

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* a(0) undefined – return 0 */

    /* --- rotations ------------------------------------ */
    seq_t rot_sum = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d = i */
            idx_t d = i;
            seq_t ph = phi_num(n / d);                /* φ(n/d) */
            seq_t Sd = count_solutions(d, 2 * d);      /* coefficient */
            rot_sum += ph * Sd;

            /* counterpart divisor n/i, if different */
            if (i != n / i)
            {
                idx_t d2 = n / i;
                seq_t ph2 = phi_num(n / d2);          /* φ(i) */
                seq_t Sd2 = count_solutions(d2, 2 * d2);
                rot_sum += ph2 * Sd2;
            }
        }
    }

    /* --- reflections ----------------------------------- */
    seq_t ref_sum = 0;

    if (n & 1)  /* n is odd */
    {
        idx_t m = (n - 1) / 2;   /* number of bead pairs */

        seq_t f0 = count_solutions(m, 2 * m);          /* sum 0 */
        seq_t f1 = count_solutions(m, 2 * m + 1);      /* sum 1 */

        ref_sum = (seq_t)n * (f0 + 2 * f1);
    }
    else  /* n is even */
    {
        /* type A reflections (two fixed beads) */
        idx_t m = (n - 2) / 2;   /* pairs after fixing two beads */

        seq_t f_neg2 = count_solutions(m, 2 * m - 2);
        seq_t f_neg1 = count_solutions(m, 2 * m - 1);
        seq_t f0     = count_solutions(m, 2 * m);
        seq_t f1     = count_solutions(m, 2 * m + 1);
        seq_t f2     = count_solutions(m, 2 * m + 2);

        /* 2*f2 + 6*f1 + 5*f0  (derived from pairCnt) */
        seq_t typeA =
            2 * f2 +
            6 * f1 +
            5 * f0;

        /* type B reflections (no fixed beads) */
        seq_t typeB = count_solutions(n / 2, n);

        ref_sum = (n / 2) * (typeA + typeB);
    }

    /* --- final division: Burnside formula --- */
    seq_t numerator   = rot_sum + ref_sum;
    seq_t denominator = (seq_t)2 * (seq_t)n;
    return numerator / denominator;
}
