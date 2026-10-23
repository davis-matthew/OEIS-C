/*
 * A008279  Triangle T(n,k) = n!/(n-k)! (0 <= k <= n) read by rows, giving
 *          number of permutations of n things k at a time.
 * https://oeis.org/A008279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy, core
 * Formula:   a(n) = falling factorial r*(r‑1)*…*(r‑c+1) where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index and c = n - r*(r+1)/2
 *            is the column offset (0‑based)
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 102, u64 n <= 249,
 *            u128 n <= 660, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column offset */
    idx_t c = n - (r * (r + 1)) / 2;

    /* Compute T(r, c) = r!/(r‑c)! = product_{i=0}^{c-1} (r‑i) */
    seq_t result = 1;
    for (idx_t i = 0; i < c; ++i)
        result *= (seq_t)(r - i);

    return result;
}
