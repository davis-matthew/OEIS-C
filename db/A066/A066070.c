/*
 * A066070  a(1) = 1; for m > 0, a(2*m) = 2*(2*m+1), a(2*m+1) = 2*m+1.
 * https://oeis.org/A066070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n if n is odd; a(n) = 2*(n+1) if n is even
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; behavior for 0 is irrelevant */
    seq_t val = (seq_t)n;
    if ((n & 1) == 0) {                 /* even n */
        val = val * 2 + 2;               /* 2*(n+1) */
    }
    return val;
}
