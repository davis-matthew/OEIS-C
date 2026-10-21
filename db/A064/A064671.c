/*
 * A064671  Number of n-digit base 4 biquanimous numbers (with leading 0's
 *          allowed, but not all-0 string).
 * https://oeis.org/A064671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2^n - 2) * (1 + 2^n - n) / 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute t = 2^n  (for n >= 1) */
    seq_t t = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        t *= (seq_t)2;

    /* factors of the product */
    seq_t f1 = t - (seq_t)2;                  /* 2^n - 2   */
    seq_t f2 = t + (seq_t)1 - (seq_t)n;       /* 1 + 2^n - n */

    /* divide the even factor by 2 to avoid overflow in
       the intermediate product */
    if ((f1 & (seq_t)1) == 0)
        f1 /= (seq_t)2;
    else
        f2 /= (seq_t)2;

    /* final result */
    return f1 * f2;
}
