/*
 * A068379  Engel expansion of sinh(1/2).
 * https://oeis.org/A068379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8 * ( n * (2*n - 3) + 1 ) for n > 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n < 2 (covers n == 1, and any n == 0 which is outside the offset) */
    if (n < 2) return (seq_t)2;

    seq_t nn = (seq_t)n;              /* cast index to seq_t type */
    seq_t tmp = nn * ((nn << 1) - 3); /* n * (2*n - 3) */
    tmp += (seq_t)1;                  /* +1 */
    return tmp << 3;                  /* multiply by 8 */
}
