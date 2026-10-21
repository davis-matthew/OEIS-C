/*
 * A094261  a(n) = n(n-1)(n-3)(n-6)...(n-t), where t is the largest triangular
 *          number less than n; number of factors in the product is
 *          ceiling((sqrt(1+8*n)-1)/2).
 * https://oeis.org/A094261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n·(n-1)·(n-3)·(n-6)·…·(n-t) where t is the largest
 *            triangular number < n The number of factors is
 *            ceil((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A094261 */
seq_t a(idx_t n)
{
    /* result begins with the factor n itself */
    seq_t res = (seq_t)n;

    /* iterate over triangular numbers T_i = i*(i+1)/2  */
    for (idx_t i = 1;; ++i) {
        idx_t tri = (i * (i + 1)) / 2;
        if (tri >= n)      /* stop when T_i is no longer smaller than n */
            break;
        /* multiply the current product by (n - T_i),
           using a 128‑bit intermediate to avoid overflow on 64‑bit builds */
        res = (seq_t)((unsigned __int128)res * ((seq_t)n - (seq_t)tri));
    }
    return res;
}
