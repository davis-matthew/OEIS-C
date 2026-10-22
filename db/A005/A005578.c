/*
 * A005578  a(2*n) = 2*a(2*n-1), a(2*n+1) = 2*a(2*n)-1.
 * https://oeis.org/A005578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*n) = 2*a(2*n-1), a(2*n+1) = 2*a(2*n)-1, a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        if (i % 2 == 0)
            v = v * 2;
        else
            v = v * 2 - 1;
    }
    return v;
}
