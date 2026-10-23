/*
 * A034941  Number of labeled triangular cacti with 2n+1 nodes (n triangles).
 * https://oeis.org/A034941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)!! * (2n+1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;

    /* compute (2n-1)!! */
    for (idx_t i = 1; i <= 2 * n - 1; i += 2)
        res = res * (seq_t)i;

    /* compute (2n+1)^(n-1) */
    seq_t base = 2 * n + 1;
    seq_t powv = 1;
    for (idx_t k = 0; k < n - 1; ++k)
        powv = powv * base;

    return res * powv;
}
