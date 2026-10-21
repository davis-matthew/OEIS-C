/*
 * A055868  Fifth column of triangle A055864.
 * https://oeis.org/A055868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1..4; a(n) = n^4*(n+1)^(n-5) for n >= 5
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    seq_t res = 1;
    seq_t n_seq = (seq_t)n;
    /* multiply by n four times: n^4 */
    res *= n_seq;
    res *= n_seq;
    res *= n_seq;
    res *= n_seq;
    /* multiply by (n+1)^(n-5) */
    seq_t base = n_seq + 1;
    idx_t exp = n - 5;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}
