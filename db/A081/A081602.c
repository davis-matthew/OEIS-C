/*
 * A081602  Number of 0's in ternary representation of n.
 * https://oeis.org/A081602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = number of zeros in ternary representation of n
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t count = 0;
    while (n > 0) {
        if ((n % 3) == 0)
            count++;
        n /= 3;
    }
    return count;
}
