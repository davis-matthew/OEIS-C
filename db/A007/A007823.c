/*
 * A007823  A007824(n)/16.
 * https://oeis.org/A007823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = D(16*a(n-1))/16, where D(Sum b_i*2^i) = Sum
 *            i*b_i*2^(i-1). Since bit k of a(n-1) is bit k+4 of 16*a(n-1),
 *            a(n) = Sum_{k: bit k of a(n-1) set} (k+4)*2^(k-1), where a k=0
 *            bit contributes 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t t = x;
        seq_t D = 0;
        seq_t p = 1;          /* 2^(k-1) for k >= 1 */
        idx_t k;
        if (t & 1) D += 2;    /* k = 0: 4 * 2^(-1) = 2 */
        t >>= 1;
        k = 1;
        while (t != 0) {
            if (t & 1) D += (seq_t)(k + 4) * p;
            t >>= 1;
            p += p;
            k++;
        }
        x = D;
    }
    return x;
}
