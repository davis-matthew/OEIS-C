/*
 * A001706  Generalized Stirling numbers, [n+4,4]_2.
 * https://oeis.org/A001706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [s^2] Product_{j=2..n+3} (s + j), from e.g.f.
 *            log(1-x)^2/(2*(1-x)^2) with offset 2; computed by multiplying a
 *            degree-2 truncated polynomial by (s+j), starting from 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c0 = 1, c1 = 0, c2 = 0;
    idx_t j;
    for (j = 2; j <= n + 3; j++) {
        seq_t m = (seq_t)j;
        c2 = c2 * m + c1;
        c1 = c1 * m + c0;
        c0 = c0 * m;
    }
    return c2;
}
