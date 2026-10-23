/*
 * A115514  Triangle read by rows: row n >= 1 lists first n positive terms of
 *          A004526 (integers repeated) in decreasing order.
 * https://oeis.org/A115514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Flatten the triangle T(n,k)=floor((n+2-k)/2); a(n) is the n‑th
 *            term of the row‑by‑row listing
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but return 0 for safety */

    /* Find the row r such that r(r+1)/2 >= n  (minimal r). */
    idx_t low  = 1;
    idx_t high = n;         /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute position within the row: pos = n - (r-1)r/2   (1 ≤ pos ≤ r). */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t pos = n - (idx_t)prev_sum;

    /* Value of the T(r, pos) entry. */
    seq_t value = ((seq_t)r + 2 - (seq_t)pos) / 2;
    return value;
}
