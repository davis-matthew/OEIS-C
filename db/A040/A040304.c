/*
 * A040304  Continued fraction for sqrt(322).
 * https://oeis.org/A040304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=17; for n≥1: a(n)=1 if n odd; otherwise let v = n & -n (the
 *            lowest power of two dividing n); a(n)=16 if v==2, else a(n)=34
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the multiplicative definition of A040304. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;
    if (n & 1U)                 /* odd */
        return (seq_t)1;
    /* even: determine exponent of 2 */
    idx_t low = n & -n;         /* lowest power of two dividing n */
    return (low == 2U) ? (seq_t)16 : (seq_t)34;
}
