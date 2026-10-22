/*
 * A006370  The Collatz or 3x+1 map: a(n) = n/2 if n is even, 3n + 1 if n is
 *          odd.
 * https://oeis.org/A006370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n/2 if n is even, 3*n+1 if n is odd (the Collatz map)
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_cast = (seq_t)n;
    if (n & 1) {
        return 3 * n_cast + 1;
    } else {
        return n_cast / 2;
    }
}
