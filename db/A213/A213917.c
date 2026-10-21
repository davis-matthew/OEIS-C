/*
 * A213917  Difference between sum of all even and the sum of all odd numbers
 *          in Collatz (3x+1) trajectory of n.
 * https://oeis.org/A213917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum of even terms in the Collatz trajectory of n minus
 *            sum of odd terms in that trajectory
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (;;) {
        if ((cur % 2) == 0)
            sum_even += cur;
        else
            sum_odd += cur;

        if (cur == 1)
            break;

        if ((cur % 2) == 0)
            cur = cur / 2;
        else
            cur = 3 * cur + 1;
    }

    return sum_even - sum_odd;
}
