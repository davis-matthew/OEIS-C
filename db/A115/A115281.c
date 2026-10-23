/*
 * A115281  Correlation triangle for the sequence 2-0^n.
 * https://oeis.org/A115281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 4*min(k,r-k)+1 if r=2k else 4*min(k,r-k)+2, where
 *            r=floor((sqrt(8*n+1)-1)/2) and k=n-r(r+1)/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;                           /* 0 <= r <= n */
    idx_t offset = (idx_t)(((seq_t)r * (r + 1)) / 2);
    idx_t k = n - offset;                     /* 0 <= k <= r */

    seq_t min_val = (k <= r - k) ? (seq_t)k : (seq_t)(r - k);
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;

    if (r_seq == 2 * k_seq)
        return 4 * min_val + 1;
    else
        return 4 * min_val + 2;
}
