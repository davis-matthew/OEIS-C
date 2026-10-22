/*
 * A000274  Number of permutations of length n with 2 consecutive ascending
 *          pairs.
 * https://oeis.org/A000274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)*D(n-1)/2, where D(m) = A000166(m) is the number of
 *            derangements: D(0) = 1, D(m) = m*D(m-1) + (-1)^m. (Equivalent to
 *            a(n) = (1/2)*([n!/e] - [(n-1)!/e]).)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    seq_t d = 1; /* D(0) */
    idx_t k;
    for (k = 1; k <= m; k++) {
        d = d * (seq_t)k;
        if (k & 1)
            d -= 1;
        else
            d += 1;
    }
    /* D(m) is even for odd m, so the halving is exact without overflow */
    if (m & 1)
        return (d / 2) * (seq_t)m;
    else
        return d * (seq_t)(m / 2);
}
