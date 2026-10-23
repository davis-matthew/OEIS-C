/*
 * A040941  Continued fraction for sqrt(973).
 * https://oeis.org/A040941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuous fraction terms for sqrt(973):
   a(0)=31; for n>0, a(n)=period[(n-1)%8] where
   period = {5,5,2,8,2,5,5,62}. */

seq_t a(idx_t n)
{
    if (n == 0) return 31;
    static const seq_t period[8] = {5,5,2,8,2,5,5,62};
    return period[(unsigned)((n - 1) % 8)];
}
