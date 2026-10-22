/*
 * A000975  a(2n) = 2*a(2n-1), a(2n+1) = 2*a(2n)+1 (also a(n) is the n-th
 *          number without consecutive equal binary digits).
 * https://oeis.org/A000975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2n) = 2*a(2n-1), a(2n+1) = 2*a(2n)+1, a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        v = v * 2;
        if (i & 1)
            v = v + 1;
    }
    return v;
}
