/*
 * A200166  Number of -n..n arrays x(0..2) of 3 elements with nonzero sum and
 *          with zero through 2 differences all nonzero.
 * https://oeis.org/A200166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*n^3 - 13*n^2 + 11*n + (n odd ? -4 : 0)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* convert index to seq_t */
    seq_t n2 = nn * nn;       /* n^2 */

    /* compute 8*n^2 + 11 - 13*n, which is always non‑negative for n >= 0 */
    seq_t tmp = 8 * n2 + 11;
    tmp -= 13 * nn;

    /* multiply by n to get 8*n^3 - 13*n^2 + 11*n */
    seq_t result = tmp * nn;

    /* add the adjustment for odd n: -4 */
    if ((n & 1) != 0)
        result -= 4;

    return result;
}
