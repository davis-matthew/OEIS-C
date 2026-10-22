/*
 * A002329  Periods of reciprocals of integers prime to 10.
 * https://oeis.org/A002329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = A084680(A045572(n)), where A045572(n) is the n-th
 *            positive integer coprime to 10, and A084680(k) is the
 *            multiplicative order of 10 modulo k (period of 1/k in decimal)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t mult_order(uint64_t base, uint64_t mod) {
    if (mod == 1) return (seq_t)1;
    uint64_t power = 1;
    for (uint64_t p = 1; p < mod; p++) {
        power = (power * base) % mod;
        if (power == 1) return (seq_t)p;
    }
    return (seq_t)mod;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        if (k % 2 != 0 && k % 5 != 0) {
            count++;
        }
    }
    return mult_order(10, k);
}
