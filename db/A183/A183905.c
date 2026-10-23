/*
 * A183905  Number of nondecreasing arrangements of n+2 numbers in 0..2 with
 *          each number being the sum mod 3 of two others.
 * https://oeis.org/A183905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 7 n - 10) / 2 for n > 1, a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + (seq_t)7);
    val -= (seq_t)10;
    return val / (seq_t)2;
}
