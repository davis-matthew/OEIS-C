/*
 * A026835  Triangular array read by rows: T(n,k) = number of partitions of n
 *          into distinct parts in which every part is >=k, for k=1,2,...,n.
 * https://oeis.org/A026835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of subsets of {c,…,r} whose sum is r, where r is
 *            the row such that r(r‑1)/2 < n ≤ r(r+1)/2 and c = n – r(r‑1)/2.
 *            This equals the number of partitions of r into distinct parts
 *            all ≥ c (the entry T(r,c))
 * Range:     u8 n <= 406, u16 n <= 3081, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Local helper: integer square root (non‑negative, floor). */
static idx_t int_sqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    /* Determine row r and column c of the flat index n. */
    idx_t r = (int_sqrt_u64(8 * n + 1) - 1) / 2;          /* floor estimate */
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;                                               /* correct if needed */
    idx_t c = n - r * (r - 1) / 2;                          /* 1‑based column */

    /* DP array for subsets of {c,…,r} summing to r.  */
    seq_t *dp = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* fallback in case of allocation failure */

    for (idx_t i = 0; i <= r; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = c; part <= r; ++part) {
        for (idx_t sum = r; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[r];
    free(dp);
    return result;
}
