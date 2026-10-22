/*
 * A002175  Excess of number of divisors of 12n+1 of form 4k+1 over those of
 *          form 4k+3.
 * https://oeis.org/A002175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = excess of divisors of 12n+1 of form 4k+1 over form 4k+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n * 12 + 1;
    seq_t count_4k1 = 0;
    seq_t count_4k3 = 0;
    
    for (idx_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            if (d % 4 == 1) {
                count_4k1++;
            } else if (d % 4 == 3) {
                count_4k3++;
            }
            
            if (d * d != m) {
                idx_t d2 = m / d;
                if (d2 % 4 == 1) {
                    count_4k1++;
                } else if (d2 % 4 == 3) {
                    count_4k3++;
                }
            }
        }
    }
    
    return count_4k1 - count_4k3;
}
