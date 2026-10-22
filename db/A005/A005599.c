/*
 * A005599  Running sum of every third term in the {+1,-1}-version of
 *          Thue-Morse sequence A010060.
 * https://oeis.org/A005599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum( (-1)^wt(3*k), k=0..n-1 ), where wt is Hamming weight
 * Range:     u8 n <= 707, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long result = 0;
    for (idx_t k = 0; k < n; k++) {
        uint64_t val = 3 * k;
        int bits = __builtin_popcountll(val);
        result += (bits & 1) ? -1 : 1;
    }
    return (seq_t)result;
}
