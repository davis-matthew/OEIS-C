/*
 * A005377  Number of low discrepancy sequences in base 4.
 * https://oeis.org/A005377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = r * (n - M(4, r)) + Sum_{h=1..r} (h-1) * N(4, h) where
 *            N(b,n) = (1/n) * Sum_{d|n} mobius(n/d) * b^d, M(b,n) =
 *            Sum_{k=1..n} N(b,k), r is the largest r such that M(b,r) <= n,
 *            and b = 4
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int mobius(idx_t n) {
    if (n == 1) return 1;
    
    int sign = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            temp /= p;
            if (temp % p == 0) return 0;
            sign = -sign;
        }
    }
    if (temp > 1) sign = -sign;
    
    return sign;
}

static seq_t N_func(idx_t n) {
    seq_t sum = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            int mu = mobius(n / d);
            if (mu != 0) {
                seq_t power = 1;
                for (idx_t i = 0; i < d; i++) {
                    power *= 4;
                }
                if (mu == 1) {
                    sum += power;
                } else {
                    sum -= power;
                }
            }
            
            if (d != n / d) {
                idx_t d2 = n / d;
                mu = mobius(n / d2);
                if (mu != 0) {
                    seq_t power = 1;
                    for (idx_t i = 0; i < d2; i++) {
                        power *= 4;
                    }
                    if (mu == 1) {
                        sum += power;
                    } else {
                        sum -= power;
                    }
                }
            }
        }
    }
    
    return sum / (seq_t)n;
}

seq_t a(idx_t n) {
    seq_t M = 0;
    idx_t r = 0;
    seq_t sum_result = 0;
    
    for (idx_t k = 1; k <= 100; k++) {
        seq_t N_k = N_func(k);
        seq_t new_M = M + N_k;
        if (new_M <= (seq_t)n) {
            M = new_M;
            r = k;
            sum_result += (seq_t)(k - 1) * N_k;
        } else {
            break;
        }
    }
    
    seq_t result = (seq_t)r * ((seq_t)n - M) + sum_result;
    
    return result;
}
