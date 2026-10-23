/*
 * A148068  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, -1), (-1,
 *          -1, 1), (-1, 1, -1), (-1, 1, 1), (1, 0, 0)}.
 * https://oeis.org/A148068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{v=0}^{⌊n/2⌋} [(n‑2v+1)/(n‑v+1)] * C(n,v) * C(v,⌊v/2⌋)²
 *            where C is the binomial coefficient. This is obtained by
 *            splitting a walk into u = n‑v up–steps (the (1,0,0) moves) and v
 *            down–steps (the four diagonal moves); the number of admissible
 *            3‑D walks equals the ballot number B(u,v) multi...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>

/*  Core function requested by the build system. */
seq_t a(idx_t n)
{
    /* the largest v needed is n/2 */
    idx_t max_v = n / 2;
    seq_t sum   = 0;

    /* binomial coefficient C(n, v) iteratively */
    seq_t binom_nv = 1;  /* C(n,0) */

    /* central binomial coefficient C(v, floor(v/2)) iteratively */
    seq_t central  = 1;  /* for v = 0 */

    for (idx_t v = 0; v <= max_v; ++v)
    {
        if (v > 0)
        {
            /* update C(n, v) */
            binom_nv = binom_nv * (seq_t)(n - v + 1) / (seq_t)v;

            /* update central C(v,⌊v/2⌋) */
            idx_t m = (v + 1) / 2;                 /* floor((v+1)/2) */
            central = central * (seq_t)v / (seq_t)m;
        }

        /* term = C(n,v) * central² * (n-2v+1) / (n-v+1) */
        seq_t term_num = binom_nv * central * central *
                         (seq_t)(n - 2 * v + 1);
        seq_t denom = (seq_t)(n - v + 1);
        seq_t term = term_num / denom;

        sum += term;
    }

    return sum;
}
