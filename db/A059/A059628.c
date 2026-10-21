/*
 * A059628  Carryless multiplication 5 X n base 10.
 * https://oeis.org/A059628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10 * a(n/10) + ((n % 10) & 1 ? 5 : 0); a(0) = 0
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return ((n & 1) ? (seq_t)5 : (seq_t)0);
    return (seq_t)10 * a(n / 10) + ((n % 10) & 1 ? (seq_t)5 : (seq_t)0);
}
