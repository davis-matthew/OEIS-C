/*
 * A005357  Number of low discrepancy sequences in base 3.
 * https://oeis.org/A005357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{h=1..r} (h-1) * N(3, h) + r * (n - M(3, r)) where
 *            N(3,k) = (1/k) * Sum_{d|k} mobius(k/d) * 3^d, M(3,k) =
 *            Sum_{i=1..k} N(3,i), r is the largest value such that M(3,r) <=
 *            n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static int mobius(uint64_t n) {
    if (n == 1) return 1;
    
    int num_primes = 0;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            num_primes++;
            n /= p;
            if (n % p == 0) return 0;
        }
    }
    if (n > 1) num_primes++;
    return (num_primes % 2 == 0) ? 1 : -1;
}

static seq_t pow3(idx_t e) {
    seq_t result = 1;
    seq_t base = 3;
    while (e > 0) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static seq_t compute_n(idx_t k) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            sum += (seq_t)mobius(k / d) * pow3(d);
            if (d != k / d) {
                sum += (seq_t)mobius(d) * pow3(k / d);
            }
        }
    }
    return sum / (seq_t)k;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    
    seq_t m_cumsum = 0;
    idx_t r = 0;
    
    for (idx_t k = 1; k <= 10000; k++) {
        seq_t n_k = compute_n(k);
        if (m_cumsum + n_k <= n) {
            m_cumsum += n_k;
            r = k;
        } else {
            break;
        }
    }
    
    seq_t result = 0;
    m_cumsum = 0;
    for (idx_t h = 1; h <= r; h++) {
        seq_t n_h = compute_n(h);
        m_cumsum += n_h;
        result += ((seq_t)h - 1) * n_h;
    }
    
    result += (seq_t)r * (n - m_cumsum);
    return result;
}
