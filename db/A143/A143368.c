/*
 * A143368  Triangle read by rows: T(n,k) is the Wiener index of a k X n grid
 *          (i.e., P_k X P_n, where P_m is the path graph on m vertices; 1 <=
 *          k <= n).
 * https://oeis.org/A143368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 76, u32 n <= 5669, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A143368: Triangle read by rows,
   T(n,k) = k*n*(n+k)*(k*n-1)/6  (k,n >= 1) */
seq_t a(idx_t idx)
{
    /* Find the row n that contains the given index */
    idx_t low = 1;
    idx_t high = idx;          /* at most idx rows */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < (unsigned __int128)idx)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t n = low;  /* row number */

    /* Offset within the row gives k */
    unsigned __int128 prev_tri = (unsigned __int128)(n - 1) * n / 2;
    idx_t k = idx - (idx_t)prev_tri;  /* 1 <= k <= n */

    /* Compute T(n,k) with 128‑bit intermediate to avoid overflow */
    unsigned __int128 prod1 = (unsigned __int128)k * n;   /* k*n          */
    unsigned __int128 prod2 = prod1 - 1;                 /* k*n - 1      */
    unsigned __int128 prod3 = (unsigned __int128)k + n;   /* n + k        */
    unsigned __int128 val = prod1 * prod2 * prod3 / 6;

    return (seq_t)val;
}
