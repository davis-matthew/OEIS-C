/*
 * A173523  1+A053735(n-1), where A053735 is the sum-of-digits function in
 *          base 3.
 * https://oeis.org/A173523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 + sum of the base‑3 digits of (n-1) (i.e. 1 +
 *            A053735(n-1))
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;          /* x = n-1 */
    seq_t sum = 0;            /* sum of base‑3 digits of x */
    while (m > 0) {
        sum += m % 3;         /* add the least significant base‑3 digit */
        m /= 3;               /* shift right by one base‑3 digit */
    }
    return sum + 1;           /* add the leading 1 as specified */
}
