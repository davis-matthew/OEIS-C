/*
 * A017778  Binomial coefficients C(62,n).
 * https://oeis.org/A017778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(62, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 62;
    if (n > N)
        return 0;

    idx_t k = n;
    if (k > N - k)
        k = N - k;          /* use symmetry C(62, n) = C(62, 62-n) */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(N - k + i) / (seq_t)i;
    }
    return res;
}
