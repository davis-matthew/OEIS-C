/*
 * A003552  Divisors of 2^47 - 1.
 * https://oeis.org/A003552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 2^47 - 1 = 2351 * 4513 * 13264529, in increasing
 *            order
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1 || n > 8) return 0;
    
    // Generate all divisors from the three prime factors of 2^47 - 1
    seq_t divisors[8];
    int idx = 0;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                seq_t d = (seq_t)1;
                if (i) d = d * (seq_t)2351;
                if (j) d = d * (seq_t)4513;
                if (k) d = d * (seq_t)13264529;
                divisors[idx++] = d;
            }
        }
    }
    
    // Sort divisors in ascending order
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
    
    return divisors[n - 1];
}
