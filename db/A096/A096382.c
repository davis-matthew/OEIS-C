/*
 * A096382  Consider a Pythagorean triangle with sides a = u^2-v^2, b = 2uv, c
 *          = u^2+v^2. The sequence is the area of the triangle when v = 2, u
 *          = 3, 4, 5, ...
 * https://oeis.org/A096382
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 8*n
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 3 are outside the required range; return 0 for safety. */
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;        /* cast index to result type */
    seq_t nnn = nn * nn * nn;   /* n³ */
    seq_t res = 2 * nnn - 8 * nn;

    return res;
}
