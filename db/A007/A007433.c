/*
 * A007433  Inverse Moebius transform applied twice to squares.
 * https://oeis.org/A007433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^2*tau(n/d)
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t count_divisors(idx_t n) {
    idx_t count = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            if (i * i != n) {
                count++;
            }
        }
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t d_sq = (seq_t)d * (seq_t)d;
            result += d_sq * (seq_t)count_divisors(n / d);
            
            if (d * d != n) {
                idx_t d2 = n / d;
                seq_t d2_sq = (seq_t)d2 * (seq_t)d2;
                result += d2_sq * (seq_t)count_divisors(d);
            }
        }
    }
    
    return result;
}
