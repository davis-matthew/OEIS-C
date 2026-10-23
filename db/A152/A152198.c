/*
 * A152198  Triangle read by rows, A007318 rows repeated.
 * https://oeis.org/A152198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r, k) * where r = floor((sqrt(1+4n)-1)/2) (row
 *            of Pascal's triangle) * and k = (n - r*(r+1)) % (r+1) (position
 *            within the repeated row)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;               /* safety, should not happen */
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1) <= n < (r+1)*(r+2) */
    idx_t m = (idx_t)((unsigned __int128)1 + (unsigned __int128)4 * n);
    idx_t s = floor_sqrt_u64(m);                   /* floor(sqrt(1+4n)) */
    idx_t r = (s - 1) / 2;                          /* initial r */
    while ((unsigned __int128)r * (r + 1) > n) r--; /* adjust if needed */

    idx_t start   = (unsigned __int128)r * (r + 1);
    idx_t offset  = n - start;                     /* 0 <= offset < 2*(r+1) */
    idx_t k       = offset % (r + 1);

    return binom_idx(r, k);
}
