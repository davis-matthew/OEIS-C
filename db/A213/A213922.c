/*
 * A213922  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement: T(n,n), T(n-1,n), T(n,n-1), ... T(1,n), T(n,1).
 *          Table T(n,k) read by antidiagonals.
 * https://oeis.org/A213922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of the index n in
 *            the enumeration by antidiagonals; T(i,j) = i*i - 2*j + 2 if i >=
 *            j, else j*j - 2*i + 1
 * Range:     u8 n <= 135, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find diagonal d such that d*(d+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (/* __int128 to avoid overflow */ ( (__int128)high * (high + 1) / 2 ) < n) {
        high <<= 1;
    }
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ( (__int128)mid * (mid + 1) / 2 ) >= n ) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t d = low;                     /* diagonal index */
    idx_t prev = (idx_t)( ( (__int128)(d - 1) * d ) / 2 ); /* entries before this diag */
    idx_t k = n - prev;                  /* position on this diagonal (1..d) */
    idx_t i = k;
    idx_t j = d + 1 - k;
    if (i >= j) {
        return (seq_t)i * (seq_t)i - (seq_t)2 * (seq_t)j + (seq_t)2;
    } else {
        return (seq_t)j * (seq_t)j - (seq_t)2 * (seq_t)i + (seq_t)1;
    }
}
