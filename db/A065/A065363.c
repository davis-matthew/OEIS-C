/*
 * A065363  Sum of balanced ternary digits in n. Replace 3^k with 1 in
 *          balanced ternary expansion of n.
 * https://oeis.org/A065363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign, look
 * Formula:   a(n) = sum of balanced ternary digits of n (each digit is -1, 0,
 *            or 1, 3ⁿ replaced by 1 in the expansion)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;

    while (x != 0) {
        idx_t r = x % 3;
        if (r == 0) {
            x = x / 3;
        } else if (r == 1) {
            sum += 1;
            x = x / 3;
        } else { /* r == 2  → digit -1 with a carry */
            sum -= 1;
            x = x / 3 + 1;
        }
    }
    return sum;
}
