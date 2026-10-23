/*
 * A185914  Array: T(n,k)=k-n+1 for k>=n; T(n,k)=0 for k<n; by antidiagonals.
 * https://oeis.org/A185914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = max(0, j‑i+1) where m = the unique integer with m*(m+1)/2
 *            ≥ n; i = n – m*(m‑1)/2 (1 ≤ i ≤ m); j = m + 1 – i. The value is
 *            the entry T(i,j) of the array T(n,k) = k‑n+1 for k ≥ n; T(n,k) =
 *            0 for k < n
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal (m) that contains the n-th element.
       m is the smallest integer with m*(m+1)/2 ≥ n. */
    idx_t low = 1;
    idx_t high = n;                     /* m can never exceed n */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        /* Use 128–bit arithmetic to avoid overflow. */
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Number of elements before this diagonal. */
    unsigned __int128 prev_sum = (unsigned __int128)(m - 1) * m / 2;
    idx_t prev = (idx_t)prev_sum;        /* fits into idx_t because it ≤ n */

    /* Position inside the diagonal (1‑based). */
    idx_t offset = n - prev;             /* 1 ≤ offset ≤ m */
    idx_t i = offset;                    /* row index for T */
    idx_t j = m + 1 - i;                 /* column index for T */

    if (j < i)
        return (seq_t)0;
    else
        return (seq_t)(j - i + 1);
}
