/*
 * A007517  a(n) = phi(n) * (sigma(n) - n).
 * https://oeis.org/A007517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) * (sigma(n) - n)
 * Range:     u8 n <= 23, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t phi(idx_t n) {
    seq_t result = (seq_t)n;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            result = result / (seq_t)p * ((seq_t)p - 1);
        }
    }
    
    if (temp > 1) {
        result = result / (seq_t)temp * ((seq_t)temp - 1);
    }
    
    return result;
}

static seq_t sigma(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            seq_t sum = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                power *= (seq_t)p;
                sum += power;
                temp /= p;
            }
            result *= sum;
        }
    }
    
    if (temp > 1) {
        result *= ((seq_t)temp + 1);
    }
    
    return result;
}

seq_t a(idx_t n) {
    return phi(n) * (sigma(n) - (seq_t)n);
}
