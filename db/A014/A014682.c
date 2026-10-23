/*
 * A014682  The Collatz or 3x+1 function: a(n) = n/2 if n is even, otherwise
 *          (3n+1)/2.
 * https://oeis.org/A014682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n/2 if n is even, otherwise (3*n+1)/2
 * Range:     u8 n <= 170, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL) {
        return ((seq_t)n) / 2;
    } else {
        return ((((seq_t)n) * (seq_t)3) + 1) / 2;
    }
}
