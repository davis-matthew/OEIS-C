/*
 * A070050  Number of Bottleneck-Monge matrices with 2 rows. In the formula
 *          below, P = 2.
 * https://oeis.org/A070050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 3*n + 16) * 2^(n - 3) - 1 (P = 2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n^2 + 3*n + 16) */
    idx_t nn = n;
    seq_t val = (seq_t)nn * (seq_t)nn + (seq_t)3 * (seq_t)nn + (seq_t)16;

    if (n >= 3) {
        /* Multiply by 2^(n-3) by left shift */
        val <<= (n - 3);
    } else {
        /* For n = 1 or 2, divide by 2^(3-n) using right shift.
           The intermediate value is divisible by the divisor. */
        val >>= (3 - n);
    }

    /* Subtract 1 to get the final result */
    val -= 1;

    return val;
}
