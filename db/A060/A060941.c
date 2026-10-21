/*
 * A060941  Duchon's numbers: the number of paths of length 5*n from the
 *          origin to the line y = 2*x/3 with unit East and North steps that
 *          stay below the line or touch it.
 * https://oeis.org/A060941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = Sum_{i=0..n} 1/(5*n+i+1) * C(5*n+1, n-i) * C(5*n+2*i, i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the binomial‑coefficient summation with exact integer division. */

#include "../utils/binom.h"

/* gcd for arbitrary seq_t types (supports operator%). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Terms for n = 0 are handled by the loop automatically. */
    seq_t sum = (seq_t)0;

    idx_t N1_base = 5 * n + (idx_t)1;   /* 5*n + 1 for the first binomial */

    for (idx_t i = (idx_t)0; i <= n; ++i) {
        /* Binomial coefficients */
        seq_t B = binom(N1_base, n - i);                       /* C(5*n+1, n-i) */
        idx_t N2 = 5 * n + (idx_t)2 * i;                      /* 5*n + 2*i */
        seq_t C = binom(N2, i);                                 /* C(5*n+2*i, i) */

        /* Denominator (5*n + i + 1) computed in idx_t to avoid overflow in small seq_t. */
        idx_t denom_idx = (idx_t)5 * n + i + (idx_t)1;
        seq_t denom = (seq_t)denom_idx;

        /* Reduce denominator with B */
        seq_t g = gcd_seq_t(B, denom);
        B /= g;
        denom /= g;

        /* Reduce remaining denominator with C */
        g = gcd_seq_t(C, denom);
        C /= g;
        denom /= g;   /* denom should now be 1 */

        /* Multiply reduced terms and accumulate */
        seq_t term = B * C;
        sum += term;
    }
    return sum;
}
