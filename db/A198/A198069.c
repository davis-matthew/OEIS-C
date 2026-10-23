/*
 * A198069  Table read by rows, T(0,0) = 1 and for n>0, 0<=k<=2^(n-1) T(n,k) =
 *          gcd(k,2^(n-1)).
 * https://oeis.org/A198069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = gcd(k, 2^(r-1)) where r is the minimal index such that n
 *            < r + 2^r and k = n - ((r-1)+2^(r-1))
 * Range:     u8 n <= 263, u16 n <= 8204, u32 n <= 8204, u64 n <= 8204,
 *            u128 n <= 8204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A198069. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Find minimal row index r such that n < r + 2^r. */
    idx_t r = 1;
    while (n >= r + ((idx_t)1ULL << r))
        r++;

    /* Offset within the row. */
    idx_t prevS = (r - 1) + ((idx_t)1ULL << (r - 1));
    idx_t k = n - prevS;

    /* Compute gcd(k, 2^(r-1)). */
    if (k == 0)
        return (seq_t)((idx_t)1ULL << (r - 1));

    /* Count trailing zeros of k. */
    idx_t tz = 0;
    idx_t tmp = k;
    while ((tmp & 1ULL) == 0)
    {
        tz++;
        tmp >>= 1;
    }
    if (tz >= (r - 1))
        tz = r - 1;

    return (seq_t)((idx_t)1ULL << tz);
}
