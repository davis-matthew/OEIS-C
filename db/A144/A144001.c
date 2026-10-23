/*
 * A144001  Rectangular array read by antidiagonals: label each unit square in
 *          the first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which x + y == 0 (mod 3); then R(m,n) is the number of
 *          unmarked squares in the rectangle [0,m] X [0,n].
 * https://oeis.org/A144001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 653, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = R(m,n) read by antidiagonals.
 * For positive integers m,n (m,n ≥ 1), R(m,n) is the number of
 * unmarked unit squares in the rectangle [0,m] × [0,n] where the
 * marking condition is x + y ≡ 0 (mod 3).  Explicitly,
 *
 *   R(m,n) = m*n - floor( 2*m*n / 3 )                 if n % 3 ≠ 1
 *   R(m,n) = m*n - floor( (2*m*n + 1) / 3 )          if n % 3 = 1
 *
 * The sequence a(n) lists R(m,n) for (m,n) in order of increasing m+n,
 * with m varying from 1 upward for a fixed sum.  Thus a(1)=R(1,1),
 * a(2)=R(1,2), a(3)=R(2,1), a(4)=R(1,3), ...
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the diagonal sum k such that (k-1)*k/2 ≥ n.  The counts of terms
     * up to diagonal k-1 is (k-2)*(k-1)/2.  Since n is at most 1e4 in the
     * intended tests, a linear search is fast enough.  Use __int128 to
     * avoid overflow in the intermediate product. */
    idx_t k = 2;
    while ( ( (__int128)(k - 1) * k ) / 2 < n ) {
        ++k;
    }

    /* Number of terms up to diagonal k-1 */
    idx_t prev = ( (__int128)(k - 2) * (k - 1) ) / 2;

    /* offset within diagonal k (1-based).  m = offset, ncoord = k - offset. */
    idx_t offset = n - prev;
    idx_t m_ = offset;
    idx_t n_ = k - offset;

    /* Compute R(m_, n_) using the formula above. */
    seq_t mn = (seq_t)m_ * (seq_t)n_;
    seq_t marked;
    if (n_ % 3 == 1) {
        marked = ((seq_t)2 * mn + 1) / 3;
    } else {
        marked = ((seq_t)2 * mn) / 3;
    }
    return mn - marked;
}
