/*
 * A185868  (Odd,odd)-polka dot array in the natural number array A000027, by
 *          antidiagonals.
 * https://oeis.org/A185868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) enumerates odd-indexed positions of the natural
   number array A000027 (Table 1.1) by antidiagonals.
   Let K be the smallest positive integer with K(K+1)/2 >= n.
   Let pos = n - (K-1)K/2 (1 <= pos <= K).
   Set r = 2*pos-1  (odd row index)
   Set c = 2*(K-pos)+1  (odd column index)
   The value in the array at (r,c) is
         V(r,c) = r(r+1)/2 + (c-1)r + (c-1)(c-2)/2   */
seq_t a(idx_t n)
{
    /* find minimal K with K*(K+1)/2 >= n */
    idx_t low = 1, high = 1;
    /* increase high until it satisfies the inequality */
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;
    /* binary search for minimal high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t K = low;
    idx_t prev = (K - 1) * K / 2;
    idx_t pos = n - prev;          /* 1-based position within the Kth antidiagonal  */

    idx_t r = 2 * pos - 1;          /* odd row index   */
    idx_t c = 2 * (K - pos) + 1;    /* odd column index */

    seq_t rt = (seq_t)r;
    seq_t ct = (seq_t)c;

    /* value = r(r+1)/2 + (c-1)*r + (c-1)(c-2)/2 */
    seq_t term = rt * (rt + 1) / 2
               + (ct - 1) * rt
               + (ct - 1) * (ct - 2) / 2;

    return term;
}
