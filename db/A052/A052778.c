/*
 * A052778  E.g.f.: x^4*log(-1/(-1+x)).
 * https://oeis.org/A052778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n ≤ 4; a(n) = n!/(n-4) for n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return 0;

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if (i == n - 4)   /* skip the (n-4) factor to keep the product small */
            continue;
        res *= (seq_t)i;
    }
    return res;
}
