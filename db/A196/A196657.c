/*
 * A196657  a(n) = 63*2^(n+1) + 1.
 * https://oeis.org/A196657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*2^(n+1) + 1
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    idx_t e = n + 1;
    while (e--) {
        val *= 2;          /* compute 2^(n+1) */
    }
    val = val * 63 + 1;   /* 63*2^(n+1) + 1 */
    return val;
}
