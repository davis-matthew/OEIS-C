/*
 * A002034  Kempner numbers: smallest positive integer m such that n divides
 *          m!.
 * https://oeis.org/A002034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = smallest m such that n divides m!
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9319 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t v_p(seq_t m, seq_t p) {
    /* Compute the p-adic valuation of m! using Legendre's formula */
    seq_t result = 0;
    seq_t power = p;
    while (power <= m) {
        result += m / power;
        if (power > m / p) break;
        power *= p;
    }
    return result;
}

static seq_t min_m_for_prime_power(seq_t p, seq_t k, seq_t upper_bound) {
    /* Find smallest m such that v_p(m!) >= k using binary search */
    seq_t lo = 0, hi = upper_bound;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (v_p(mid, p) >= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
/* Uses Kempner's algorithm: for each prime power p^k in n's factorization,
   find smallest m such that v_p(m!) >= k, where v_p is the p-adic valuation */

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    if (n == 1) return 1;
    
    seq_t result = 0;
    seq_t temp = n;
    
    /* Process each prime factor of n */
    for (seq_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            seq_t k = 0;
            while (temp % p == 0) {
                k++;
                temp /= p;
            }
            seq_t m = min_m_for_prime_power(p, k, n);
            if (m > result) result = m;
        }
    }
    
    /* If temp > 1, it's a prime factor with exponent 1 */
    if (temp > 1) {
        if (temp > result) result = temp;
    }
    
    return result;
}
