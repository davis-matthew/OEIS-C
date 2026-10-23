/*
 * A025586  Largest value in '3x+1' trajectory of n.
 * https://oeis.org/A025586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = largest value in the 3x+1 (Collatz) trajectory of n,
 *            ending when 1 is reached
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t max = cur;

    while (cur != 1) {
        if ((cur & 1) == 0)
            cur = cur >> 1;
        else
            cur = 3 * cur + 1;
        if (cur > max)
            max = cur;
    }
    return max;
}
