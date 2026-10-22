/*
 * A003534  Divisors of 2^26 - 1.
 * https://oeis.org/A003534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 2^26 - 1 in ascending order
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = 67108863;  /* 2^26 - 1 */
    seq_t divisors[16];
    idx_t count = 0;
    
    /* Find all divisors by trial division */
    for (seq_t i = 1; i <= N / i; i++) {
        if (N % i == 0) {
            divisors[count++] = i;
            seq_t other = N / i;
            if (i != other) {
                divisors[count++] = other;
            }
        }
    }
    
    /* Sort divisors in ascending order */
    for (idx_t i = 0; i < count - 1; i++) {
        for (idx_t j = i + 1; j < count; j++) {
            if (divisors[i] > divisors[j]) {
                seq_t temp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = temp;
            }
        }
    }
    
    /* Return n-th divisor (1-indexed) */
    if (n >= 1 && n <= count) {
        return divisors[n - 1];
    }
    return 0;
}
