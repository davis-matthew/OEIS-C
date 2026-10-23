/*
 * A030192  Scaled Chebyshev U-polynomial evaluated at sqrt(6)/2.
 * https://oeis.org/A030192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2); a(0) = 1; a(1) = 6;
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A030192 using the linear recurrence. */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Iteratively compute subsequent terms. */
    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)6;   /* a(n-1) */
    seq_t ai = a1;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)6 * a1 - (seq_t)6 * a0;  /* a(i) = 6*a(i-1) - 6*a(i-2) */
        a0 = a1;
        a1 = ai;
    }

    return ai;
}
