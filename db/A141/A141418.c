/*
 * A141418  Triangle read by rows: T(n,k) = k * (2*n - k - 1) / 2, 1 <= k <=
 *          n.
 * https://oeis.org/A141418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k * (2*r - k - 1) / 2, * where r =
 *            ceil((sqrt(8*n+1)-1)/2) and k = n - r * (r-1) / 2
 * Range:     u8 n <= 293, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1 only */

    /* Find the smallest r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* row number (1‑based) */

    /* Number of terms before this row */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;     /* column within the row (1‑based) */

    /* Compute the triangle entry */
    seq_t val = (seq_t)k * ((seq_t)(2 * (seq_t)r) - (seq_t)k - 1) / 2;
    return val;
}
