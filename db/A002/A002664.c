/*
 * A002664  a(n) = 2^n - C(n,0) - ... - C(n,4).
 * https://oeis.org/A002664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + C(n-1,4), a(0) = 0; the binomials C(m,k) are
 *            updated with Pascal's rule
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    seq_t c1 = 0, c2 = 0, c3 = 0, c4 = 0; /* C(m,1..4) for m = 0 */
    idx_t m;
    for (m = 0; m < n; m++) {
        r = r + r + c4;      /* a(m+1) = 2*a(m) + C(m,4) */
        c4 += c3;            /* C(m+1,k) = C(m,k) + C(m,k-1) */
        c3 += c2;
        c2 += c1;
        c1 += 1;
    }
    return r;
}
