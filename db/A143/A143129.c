/*
 * A143129  Triangle read by rows, T(n,k) = sum {j=k..n} A000292(j) = A000012
 *          * (A000292 * 0^(n-k)) * A000012, 1<=k<=n.
 * https://oeis.org/A143129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r+3,4) - binom(k+2,4) * where r = smallest integer
 *            with r*(r+1)/2 >= n * and k = n - r*(r-1)/2 (1‑based row and
 *            column indices)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom4(idx_t x)
{
    if (x < 4) return (seq_t)0;
    /* use 128‑bit intermediate to avoid overflow for 32‑bit seq_t */
    unsigned __int128 prod = (unsigned __int128)x;
    prod *= (unsigned __int128)(x - 1);
    prod *= (unsigned __int128)(x - 2);
    prod *= (unsigned __int128)(x - 3);
    prod /= 24;
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n )
        r++;

    /* position k within row r */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* compute the binomial terms */
    seq_t total_r   = binom4(r + 3);  /* binom(r+3,4)  */
    seq_t total_km1 = binom4(k + 2);  /* binom(k+2,4)  */

    return total_r - total_km1;
}
