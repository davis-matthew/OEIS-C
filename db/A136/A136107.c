/*
 * A136107  Number of representations of n as the difference of two positive
 *          triangular numbers.
 * https://oeis.org/A136107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (u, v) of positive integers with u*v =
 *            2*n, u < v, u and v of opposite parity, and v - u >= 3.
 *            Equivalently, the number of representations of n as the
 *            difference of two positive triangular numbers
 * Range:     u8 n <= 9181, u16 n <= 9181, u32 n <= 9181, u64 n <= 9181,
 *            u128 n <= 9181, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 2 * n;          /* m = 2*n, fits in idx_t for all n considered */
    seq_t count = 0;          /* result type seq_t, safe for any n up to 64-bit */

    /* enumerate divisor pairs u <= v of m */
    for (idx_t u = 1; u <= m / u; ++u) {
        if (m % u != 0) continue;   /* u not a divisor of m */

        idx_t v = m / u;            /* complementary divisor */

        if (u == v) continue;       /* equal divisors give no representation */

        /* u < v is guaranteed by u <= m/u and u != v, but check parity */
        if (((u & 1U) == (v & 1U))         /* same parity -> reject */
            || (v - u < 3))               /* difference too small -> reject */
            continue;

        /* now u and v satisfy all conditions */
        ++count;
    }
    return count;
}
