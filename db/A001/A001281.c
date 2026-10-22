/*
 * A001281  Image of n under the map n->n/2 if n even, n->3n-1 if n odd.
 * https://oeis.org/A001281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n even, a(n) = 3*n - 1 if n odd
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n & 1) {
        return 3 * sn - 1;
    } else {
        return sn / 2;
    }
}
