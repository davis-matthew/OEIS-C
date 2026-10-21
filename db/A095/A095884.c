/*
 * A095884  Triangle read by rows: T(n,k) = (n-k)^k, n>=1, 1<=k<=n.
 * https://oeis.org/A095884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r-k)^k, where r = smallest integer with r*(r+1)/2 >= n
 *            and k = n - r*(r-1)/2 (1‑based indexing, offset 1)
 * Range:     u8 n <= 31, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r :  S(r-1) < n <= S(r),  S(r)=r*(r+1)/2. */
    idx_t r   = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;          /* sum == S(r) after the loop */
    }

    /* Column k (1‑based) within that row. */
    idx_t k = n - (sum - r);   /* sum - r  == S(r-1) */

    /* Compute (r-k)^k.  Use simple repeated multiplication; this
       is safe for 64‑bit builds because if the final value fits in
       seq_t then all intermediate results fit as well. */
    seq_t base = (seq_t)(r - k);
    if (base == (seq_t)0) {
        return (seq_t)0;
    }
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        result *= base;
    }
    return result;
}
