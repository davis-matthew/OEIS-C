/*
 * A038714  Pronic numbers repeated 4 times; a(n) = floor(n/4) *
 *          ceiling((n+1)/4).
 * https://oeis.org/A038714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4) * (floor(n/4) + 1)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q    = n / 4;      /* floor(n/4) in integer arithmetic */
    seq_t k    = (seq_t)q;   /* cast to seq_t once we have the correct value */
    return k * (k + 1);      /* k * (k+1) gives a(n) */
}
