/*
 * A131739  a(4n) = a(4n+1) = n, a(4n+2) = 3n+2, a(4n+3) = 3n+3.
 * https://oeis.org/A131739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4n) = a(4n+1) = n, a(4n+2) = 3n+2, a(4n+3) = 3n+3
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* k = n / 4 (integer division) */
    idx_t k = n >> 2;
    /* convert to seq_t to avoid overflow when multiplying by 3 */
    seq_t ks = (seq_t)k;
    switch ((int)(n & 3)) {
        case 0: /* a(4k)  */
        case 1: /* a(4k+1) */
            return ks;
        case 2: /* a(4k+2) */
            return 3 * ks + 2;
        case 3: /* a(4k+3) */
            return 3 * ks + 3;
    }
    return 0; /* unreachable */
}
