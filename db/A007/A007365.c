/*
 * A007365  Smallest k such that sigma(n+k) = sigma(k).
 * https://oeis.org/A007365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   smallest k such that sigma(n+k) = sigma(k)
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma(seq_t x) {
    seq_t sum = 0;
    for (seq_t i = 1; i * i <= x; i++) {
        if (x % i == 0) {
            sum += i;
            if (i != x / i) {
                sum += x / i;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t k = 1;
    while (1) {
        if (sigma((seq_t)n + k) == sigma(k)) {
            return k;
        }
        k++;
    }
}
