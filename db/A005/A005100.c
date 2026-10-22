/*
 * A005100  Deficient numbers: numbers k such that sigma(k) < 2k.
 * https://oeis.org/A005100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterates to find the n-th deficient number k where sigma(k) < 2k */

static seq_t sigma(seq_t k) {
    seq_t sum = 0;
    for (seq_t i = 1; i <= k / i; i++) {
        if (k % i == 0) {
            sum += i;
            if (i != k / i) {
                sum += k / i;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 1;
    
    while (1) {
        if (sigma(k) < 2 * k) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
