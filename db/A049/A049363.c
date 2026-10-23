/*
 * A049363  a(1) = 1; for n > 1, smallest digitally balanced number in base n.
 * https://oeis.org/A049363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = 1*n^(n-1) + Σ_{m=2}^{n-1} m*n^(n-1-m)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, returned 0 */
    seq_t res = 0;

    /* first digit 1 */
    res = res * (seq_t)n + (seq_t)1;

    /* second digit 0 */
    res = res * (seq_t)n + (seq_t)0;

    /* remaining digits 2 .. n-1 */
    for (idx_t m = 2; m < n; ++m)
        res = res * (seq_t)n + (seq_t)m;

    return res;
}
