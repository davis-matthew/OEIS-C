/*
 * A122844  Triangle read by rows: T[n,k] = the number of ascending runs of
 *          length at least k in the permutations of [n] for k <= n.
 * https://oeis.org/A122844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T[r,k] where r is the unique integer with * r*(r-1)/2 < n
 *            <= r*(r+1)/2, k = n - r*(r-1)/2, and * T[r,k] = r! *
 *            (k*(r-k+1)+1) / (k+1)!
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* Binary search for the row r.  r <= n, so no overflow. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 m = (unsigned __int128)mid * (mid + 1) / 2;
        if (m >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* number of elements before row r */
    unsigned __int128 prev_cum128 = (unsigned __int128)r * (r - 1) / 2;
    idx_t prev_cum = (idx_t)prev_cum128;   /* fits in idx_t */
    idx_t k = n - prev_cum;                /* 1‑based column index */

    /* T[r,r] is always 1 */
    if (k == r) return 1;

    /* Compute r! / (k+1)! via product from i=k+2 to r */
    seq_t result = 1;
    for (idx_t i = k + 2; i <= r; ++i)
        result *= (seq_t)i;

    seq_t num = (seq_t)k * (seq_t)(r - k + 1) + 1;
    result *= num;
    return result;
}
