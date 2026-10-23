/*
 * A032434  Triangle read by rows: last survivors of Josephus elimination
 *          process.
 * https://oeis.org/A032434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) enumerates the Josephus survivor table T(n,k) read
 *            row‑wise. T(1,k)=1; T(n,k) = (T(n-1,k)+k)%n; if zero then n.
 *            a(n) is T(r,k) where r is the smallest integer with r*(r+1)/2 >=
 *            n, k = n - (r-1)*r/2
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Locate the row r that contains the n‑th term. */
    idx_t low  = 1;
    idx_t high = n;                     /* r can never exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sum = mid * (mid + 1) / 2; /* fits in 64‑bit for all practical n */
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index within that row. */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                  /* 1‑based */

    /* Compute the Josephus survivor T(r,k). */
    seq_t v = 1;                         /* T(1,k) */
    for (idx_t i = 2; i <= r; ++i) {
        v = (v + (seq_t)k) % (seq_t)i;   /* (T(i-1,k)+k) mod i */
        if (v == 0) v = i;               /* convert 0 to i per definition */
    }
    return v;
}
