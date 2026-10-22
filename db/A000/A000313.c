/*
 * A000313  Number of permutations of length n with 3 consecutive ascending
 *          pairs.
 * https://oeis.org/A000313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*A000166(n-2)/6 for n >= 2, a(1) = 0, where
 *            A000166(m) = m*A000166(m-1) + (-1)^m, A000166(0) = 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t d, c;
    idx_t m;

    if (n < 3)
        return (seq_t)0;

    /* d = A000166(n-2), the number of derangements of n-2 elements */
    d = (seq_t)1;
    for (m = 1; m <= n - 2; m++) {
        d = d * (seq_t)m;
        if (m % 2 == 0)
            d += 1;
        else
            d -= 1;
    }

    /* c = (n-2)*(n-1)/2, which is always an integer */
    c = (seq_t)(n - 2) * (seq_t)(n - 1) / 2;

    if (c % 3 == 0)
        return (c / 3) * d;
    return (d / 3) * c;
}
