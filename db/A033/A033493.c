/*
 * A033493  Sum of the numbers in the trajectory of n for the 3x+1 problem.
 * https://oeis.org/A033493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of numbers in the Collatz (3x+1) trajectory starting
 *            at n and ending at 1
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cur = (seq_t)n;
    while (1) {
        sum += cur;
        if (cur == 1)
            break;
        if (cur & 1)
            cur = 3 * cur + 1;
        else
            cur >>= 1;
    }
    return sum;
}
