/*
 * A003982  Table read by rows: 1 if x = y, 0 otherwise, where (x,y) =
 *          (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),...
 * https://oeis.org/A003982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, nice, easy
 * Formula:   a(n) = 1 if (x,y) pair has x == y (reading pairs by
 *            antidiagonals), 0 otherwise. Equivalently: a(n) =
 *            floor(sqrt(2*n+1)) - floor(sqrt(2*n))
 * Range:     u8 n <= 9127, u16 n <= 9127, u32 n <= 9127, u64 n <= 9127,
 *            u128 n <= 9127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find row k (sum x+y) such that k(k+1)/2 <= n < (k+1)(k+2)/2
    idx_t left = 0, right = (1ULL << 33);
    while (left < right) {
        idx_t mid = (left + right + 1) / 2;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    idx_t k = left;
    
    // Position within the row
    unsigned __int128 tri_k = ((unsigned __int128)k * (k + 1)) / 2;
    idx_t pos = n - (idx_t)tri_k;
    
    // Output 1 if k is even and pos == k/2, else 0
    return ((k & 1) == 0 && 2 * pos == k) ? (seq_t)1 : (seq_t)0;
}
