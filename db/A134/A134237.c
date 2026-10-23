/*
 * A134237  Triangle read by rows, a(1) = 1, n-th row n terms of: (2n-1, 2n,
 *          2n+1, ..., followed by n).
 * https://oeis.org/A134237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r + k – 2 for k < r a(n) = r for k = r where r =
 *            smallest integer with r*(r+1)/2 ≥ n and k = n – (r−1)*r/2
 *            (1‑based position in row r)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)              /* external linkage, 1‑based index */
{
    /* --- find the row r containing index n --- */
    idx_t low = 1, high = n;          /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                      /* row number */

    /* --- compute position k within the row --- */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;   /* terms before this row */
    idx_t k = n - (idx_t)prev_sum;                  /* 1‑based position in row r */

    if (k < r)
        return (seq_t)(2 * r + k - 2);   /* values 2r‑1, 2r, … */
    else
        return (seq_t)r;                 /* the last term of the row */
}
