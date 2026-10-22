/*
 * A003526  Divisors of 2^15 - 1.
 * https://oeis.org/A003526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 2^15 - 1 = 32767 = 7 * 31 * 151
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
        
    if (n < 1) return 0;
    
    const seq_t m = 32767;
    seq_t divisors[10];
    idx_t count = 0;
    
    for (seq_t i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            divisors[count++] = i;
            if (i != m / i) {
                divisors[count++] = m / i;
            }
        }
    }
    
    if (n > count) return 0;
    
    for (idx_t i = 0; i < count; i++) {
        for (idx_t j = i + 1; j < count; j++) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
    
    return divisors[n - 1];
}
