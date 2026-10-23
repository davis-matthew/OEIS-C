/*
 * A136320  Terms of A047241 swapped in pairs.
 * https://oeis.org/A136320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor(n/2) + (n % 2 ? 1 : 3) (a(0)=3, a(1)=1 ... )
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = (seq_t)(n / 2);
    return 6 * half + ((n & 1) ? 1 : 3);
}
