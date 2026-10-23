/*
 * A199944  Number of -n..n arrays x(0..2) of 3 elements with zeroth through
 *          2nd differences all nonzero.
 * https://oeis.org/A199944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*n - 10*n^2 + 8*n^3   for even n
 *            a(n) = -2 + 6*n - 10*n^2 + 8*n^3 for odd n
 * (0 ≤ n,  a(1) = 2,  a(2) = 36,  a(3) = 142, …)
 */
seq_t a(idx_t n)
{
    /* convert n to seq_t to avoid mixing types */
    seq_t sn = (seq_t)n;

    /* precompute powers of n to keep intermediate values small */
    seq_t n2 = sn * sn;   /* n^2 */
    seq_t n3 = n2 * sn;   /* n^3 */

    /* base formula 6*n - 10*n^2 + 8*n^3 */
    seq_t res = 8 * n3 - 10 * n2 + 6 * sn;

    /* adjust for odd n */
    if ((n & 1U) != 0)
        res -= 2;

    return res;
}
