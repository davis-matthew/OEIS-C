/*
 * A121378  EBCDIC codes for decimal digits.
 * https://oeis.org/A121378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 240 + (n-1)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is a 1-based index; sequence is 240, 241, 242, ... */
    return (seq_t)240 + (seq_t)(n - 1);
}
