/*
 * A135858  A128229^2 * A000012.
 * https://oeis.org/A135858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the least integer with r*(r+1)/2 ≥ n. Let k = n -
 *            r*(r-1)/2 (position inside row r). Then if r == 1 → a(n) = 1
 *            else if k ≤ r-2 → a(n) = r^2 – r + 1 else if k == r-1 → a(n) =
 *            2*r – 1 else → a(n) = 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binary search to find r */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1); /* mid*(mid+1) */
        if (prod >= (unsigned __int128)2 * n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* index within the row */
    unsigned __int128 prev_sum = (unsigned __int128)r * (r - 1) / 2;  /* (r-1)*r/2 */
    idx_t k = n - (idx_t)prev_sum;

    if (r == 1)
        return (seq_t)1;

    if (k <= r - 2) {
        unsigned __int128 val = (unsigned __int128)r * r - r + 1;      /* r^2 – r + 1 */
        return (seq_t)val;
    } else if (k == r - 1) {
        return (seq_t)((unsigned __int128)2 * r - 1);
    } else {
        return (seq_t)1;
    }
}
