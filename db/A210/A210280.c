/*
 * A210280  a(n) = (7n)!/7^n.
 * https://oeis.org/A210280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)!/7^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return 1;

    idx_t      rem7   = n;                 /* number of 7's still to cancel */
    seq_t      result = 1;
    idx_t      limit  = 7 * n;             /* upper bound of the product */

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t x = (seq_t)i;                /* current factor                  */
        while (rem7 > 0 && (x % 7) == 0) {
            x /= 7;
            rem7--;
        }
        result *= x;
    }

    return result;
}
