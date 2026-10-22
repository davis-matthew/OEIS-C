/*
 * A000853  Number of n-input 2-output switching networks under action of
 *          complementing group C(2,n) on inputs and S(2) and C(2,2) on
 *          outputs.
 * https://oeis.org/A000853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(2^n)+6*(2^n-1)*4^(2^(n-1))+2^(2^n+1))/2^(n+3),
 *            evaluated as 2^(2N-n-3) + 6*(N-1)*2^(N-n-3) + 2^(N-n-2) with N =
 *            2^n (n >= 3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pw2(idx_t e) {
    seq_t r = (seq_t)1;
    seq_t b = (seq_t)2;
    while (e > 0) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e > 0) b = b * b;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)18;
    idx_t N = (idx_t)1 << n;
    seq_t t1 = pw2(2 * N - n - 3);
    seq_t t2 = (seq_t)6 * (seq_t)(N - 1) * pw2(N - n - 3);
    seq_t t3 = pw2(N - n - 2);
    return t1 + t2 + t3;
}
