/*
 * A155724  Triangle read by rows: T(n, k) = 2*n*k + n + k - 4.
 * https://oeis.org/A155724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*r*k + r + k - 4 where r = minimal integer with
 *            r*(r+1)/2 >= n and k = n - (r-1)*r/2 (1‑based position in row r)
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 for safety */

    /* Find the row number r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Number of terms before this row */
    idx_t prev = (r - 1) * r / 2;

    /* Position k within the row (1‑based) */
    idx_t k = n - prev;

    /* Compute the term using seq_t arithmetic */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    return 2 * rn * kn + rn + kn - 4;
}
