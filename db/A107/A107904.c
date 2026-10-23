/*
 * A107904  Expansion of (1+6x)/(1-12x^2).
 * https://oeis.org/A107904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 12^n, a(2n+1) = 6*12^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 6, a(n) = 12 * a(n-2) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Compute 12^(n/2) */
    idx_t exp = n >> 1;          /* n/2 */
    seq_t pow12 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow12 *= (seq_t)12;

    if ((n & 1) == 0) /* even */
        return pow12;
    else              /* odd */
        return pow12 * (seq_t)6;
}
