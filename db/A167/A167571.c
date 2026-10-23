/*
 * A167571  The row sums of triangle A167569.
 * https://oeis.org/A167571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (4^n - 1) / (2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t
gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not needed for offset 1, but harmless */

    /* compute n! */
    seq_t fac = 1;
    for (idx_t i = 1; i <= n; ++i)
        fac *= (seq_t)i;

    /* compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    seq_t diff = pow4 - (seq_t)1;
    seq_t denom = (seq_t)(2 * n + 1);

    /* cancel common factors with denom */
    seq_t g1 = gcd_seq(diff, denom);
    diff /= g1;
    denom /= g1;

    seq_t g2 = gcd_seq(fac, denom);
    fac /= g2;
    denom /= g2;   /* should be 1 after the two cancellations */

    return fac * diff;
}
