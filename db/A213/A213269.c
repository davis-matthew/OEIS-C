/*
 * A213269  The number of edges in the directed graph of the 2-opt landscape
 *          of the symmetric TSP.
 * https://oeis.org/A213269
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n-3) / 4 (n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)           /* a(3) = 0, values below are undefined */
        return 0;

    /* For n = 3 the result is 0, the formula also gives 0 */
    if (n == 3)
        return 0;

    /* Count the total number of factors of 2 in n!   */
    idx_t twos = 0;
    for (idx_t d = 2; d <= n; d <<= 1)
        twos += n / d;

    /* Compute the product of all odd factors of n! */
    seq_t odd = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t v = i;
        /* Strip all factors of 2 from i */
        while ((v & 1) == 0)
            v >>= 1;
        odd *= (seq_t)v;
    }

    /* Combine: a(n) = odd * (n-3) * 2^(twos-2)  */
    seq_t res = odd * (seq_t)(n - 3);

    idx_t shift = twos - 2;          /* twos ≥ 2 for n ≥ 4 */
    if (shift > 0)
        res <<= (unsigned)shift;     /* multiply by 2^(twos-2) */

    return res;
}
