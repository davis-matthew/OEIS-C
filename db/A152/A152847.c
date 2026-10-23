/*
 * A152847  Triangle read by rows, A007318 rows repeated nine times.
 * https://oeis.org/A152847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(r, k) where r is the unique integer such that
 *            9*r*(r+1)/2 <= n < 9*(r+1)*(r+2)/2, and k = (n - 9*r*(r+1)/2) %
 *            (r+1). A007318 is Pascal's triangle
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_row(idx_t r, idx_t k)
{
    if (k > r) return 0;
    if (k > r - k) k = r - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = (result * (seq_t)(r - k + i)) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine the row r. */
    idx_t r = 0;
    while (1)
    {
        __int128 t = (__int128)9 * (r + 1) * (r + 2);
        if ((idx_t)(t / 2) > n)
            break;
        ++r;
    }

    /* Number of terms preceding row r. */
    __int128 t_before = (__int128)9 * r * (r + 1);
    idx_t terms_before = (idx_t)(t_before / 2);

    /* Position inside row r. */
    idx_t offset = n - terms_before;

    /* Which coefficient of row r. */
    idx_t k = offset % (r + 1);

    return binomial_row(r, k);
}
