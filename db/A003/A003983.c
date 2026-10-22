/*
 * A003983  Array read by antidiagonals with T(n,k) = min(n,k).
 * https://oeis.org/A003983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   a(n) = min(i,j) where (i,j) is the n-th position in antidiagonal
 *            reading of array T(i,j)=min(i,j)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t n_u64 = (uint64_t)n;
    
    // Binary search for diagonal d such that d*(d+1)/2 >= n
    uint64_t lo = 1, hi = 1ULL << 33;
    
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        uint64_t tri = mid * (mid + 1) / 2;
        
        if (tri < n_u64) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    
    uint64_t d = lo;
    
    // Position within diagonal d (1-indexed)
    uint64_t pos = n_u64 - d * (d - 1) / 2;
    
    // Antidiagonal d has elements (1,d), (2,d-1), ..., (d,1)
    // Position pos corresponds to (pos, d-pos+1)
    uint64_t col = d - pos + 1;
    
    return (seq_t)(pos < col ? pos : col);
}
