/*
 * A155705  Triangle read by rows where T(m,n) = 2*m*n + m + n + 2.
 * https://oeis.org/A155705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*m*k + m + k + 2, where m is the smallest integer with
 *            m(m+1)/2 >= n, and k = n - (m-1)m/2 (the position in row m)
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find m such that triangular number >= n using binary search. */
    idx_t low = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute the rank within row m. */
    idx_t prev = (idx_t)((unsigned __int128)(m - 1) * m / 2); /* terms before row m */
    idx_t k   = n - prev;                                   /* 1-based index in row m */

    /* Compute the triangle value. */
    seq_t val = 2 * ((seq_t)m * (seq_t)k) + (seq_t)m + (seq_t)k + 2;
    return val;
}
