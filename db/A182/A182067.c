/*
 * A182067  a(n) = floor(n) - floor(n/2) - floor(n/3) - floor(n/5) +
 *          floor(n/30).
 * https://oeis.org/A182067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n/2) - floor(n/3) - floor(n/5) + floor(n/30)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute in idx_t to avoid overflow, then cast to seq_t */
    idx_t v = n - n/2 - n/3 - n/5 + n/30;
    return (seq_t)v;
}
