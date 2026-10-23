/*
 * A107844  Highest value obtained in the recursion of McCarthy's 91 function
 *          until it terminates.
 * https://oeis.org/A107844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 111 for n <= 100, a(n) = n for n > 100
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n <= 100) ? (seq_t)111 : (seq_t)n;
}
