/*
 * A115147  Eighth convolution of A115140.
 * https://oeis.org/A115147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   for n>=8, a(n) = -8*binomial(2*n-9, n-8)/n. For n<8, a(0)=1,
 *            a(1)=-8, a(2)=20, a(3)=-16, a(4)=2, a(5)=0, a(6)=0, a(7)=0
 * Range:     i8 n <= 10, i16 n <= 14, i32 n <= 22, i64 n <= 39, i128 n <= 70,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* initial values up to n = 7 */
    static const seq_t init[8] = {
        (seq_t)1,    /* a(0) */
        (seq_t)-8,   /* a(1) */
        (seq_t)20,   /* a(2) */
        (seq_t)-16,  /* a(3) */
        (seq_t)2,    /* a(4) */
        (seq_t)0,    /* a(5) */
        (seq_t)0,    /* a(6) */
        (seq_t)0     /* a(7) */
    };

    if (n < 8)
        return init[(size_t)n];

    /* For small seq_t types (≤64 bits) compute without overflowing 64‑bit
       arithmetic by using a 128‑bit intermediate. */
    if (sizeof(seq_t) <= 8) {
        idx_t r = n - 8;           /* r = n-8 */
        idx_t N = 2 * n - 9;       /* N = 2n-9 */
        __int128 bin = 1;          /* binomial(N, r) */

        for (idx_t i = 1; i <= r; ++i) {
            bin = bin * (N - r + i); /* multiply by numerator */
            bin /= i;                /* exact division by denominator */
        }

        bin *= ( __int128)8;  /* multiply by 8 */
        bin /= ( __int128)n;  /* divide by n (exact) */
        bin = -bin;           /* final sign */

        return (seq_t)bin;
    }

    /* For arbitrary‑precision seq_t use the provided binom routine. */
    idx_t r = n - 8;
    idx_t N = 2 * n - 9;

    seq_t bin = binom(N, r);
    seq_t val = (bin * ((seq_t)8)) / ((seq_t)n);
    return -val;
}
