/*
 * A110592  Number of digits in base-5 representation of n. String length of
 *          A007091.
 * https://oeis.org/A110592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 for n = 0; otherwise a(n) = floor(log_5(n)) + 1
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t digits = 0;
    idx_t tmp = n;

    while (tmp > 0) {
        /* Count one digit */
        digits++;
        /* Remove the least significant base-5 digit */
        tmp /= 5;
    }

    return digits;
}
