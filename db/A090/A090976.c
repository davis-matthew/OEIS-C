/*
 * A090976  a(n) = 100 reduced mod n.
 * https://oeis.org/A090976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100 mod n; for n > 100 the result is 100
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 100) {
        return (seq_t)(100 % (idx_t)n);
    } else {
        return (seq_t)100;
    }
}
