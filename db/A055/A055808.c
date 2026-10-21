/*
 * A055808  a(n) and floor(a(n)/4) are both squares; i.e., squares that remain
 *          squares when written in base 4 and last digit is removed.
 * https://oeis.org/A055808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=4*(n-1)^2 for n>1
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    /* Compute 4*(n-1)^2, casting n-1 to seq_t first */
    seq_t t = (seq_t)(n - 1);
    return (seq_t)4 * t * t;
}
