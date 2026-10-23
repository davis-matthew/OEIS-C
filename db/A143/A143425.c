/*
 * A143425  Triangle read by rows A051731 * A130123, 1<=k<=n.
 * https://oeis.org/A143425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(k-1) if k divides r, otherwise 0; r is the unique
 *            integer with r(r-1)/2 < n ≤ r(r+1)/2, and k = n - r(r-1)/2
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(seq_t exp)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= exp; ++i)
        val *= 2;
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1, guard for 0 */

    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    if (r % k == 0)
        return pow2_exp((seq_t)(k - 1));
    return 0;
}
