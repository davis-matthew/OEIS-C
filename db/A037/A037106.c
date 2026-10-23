/*
 * A037106  Trajectory of 3 under map n->17n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(0)=3; a(n) = { 17*a(n-1)+1, if a(n-1) is odd a(n-1)/2, if
 *            a(n-1) is even }
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 48, u64 n <= 73, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)3;
    for (idx_t i = 0; i < n; ++i) {
        /* use bitwise odd test for speed and portability */
        if ((cur & 1) == (seq_t)1) {
            cur = cur * (seq_t)17 + (seq_t)1;
        } else {
            cur = cur / (seq_t)2;
        }
    }
    return cur;
}
