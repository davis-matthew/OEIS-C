/*
 * A173527  a(n) = 1 + A053828(n-1), where A053828 is the sum of digits in
 *          base 7.
 * https://oeis.org/A173527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + sum_of_digits_base7(n-1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the behaviour is irrelevant, but guard against underflow. */
    if (n == 0) {
        return 0;
    }

    idx_t x = n - 1;          /* value whose base‑7 digits are summed */
    seq_t sum = 0;            /* cumulative digit sum */

    while (x != 0) {
        sum += (seq_t)(x % 7); /* add least significant base‑7 digit */
        x   /= 7;             /* shift right one digit in base 7 */
    }

    return sum + 1;           /* final term of the sequence */
}
