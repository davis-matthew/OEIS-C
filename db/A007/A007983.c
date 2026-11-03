/*
 * A007983  Number of non-Abelian metacyclic groups of order p^n (p odd).
 * https://oeis.org/A007983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Derived from pattern in differences d(n) = a(n+1) - a(n);
 *            computes a(n) by accumulating d values
 * Range:     u8 n <= 23, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 946 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_floor_sum(idx_t n) {
    if (n <= 2) return 0;
    
    idx_t q_prime = (n - 2) / 3;
    idx_t r_prime = (n - 2) % 3;
    
    seq_t result = ((seq_t)(q_prime + 1) * 3 * (seq_t)q_prime) / 2 + (seq_t)(q_prime + 1) * (seq_t)r_prime;
    return result;
}

static seq_t b_seq(idx_t k) {
    if (k == 0) return 0;
    return (seq_t)(k - 1) + compute_floor_sum(k);
}

static seq_t c_seq(idx_t k) {
    if (k == 0) return 0;
    return (seq_t)k + compute_floor_sum(k - 1);
}

seq_t a(idx_t n) {
    if (n == 1 || n == 2) return 0;
    
    seq_t sum = 0;
    if (n % 2 == 0) {
        idx_t m = n / 2;
        for (idx_t j = 1; j <= m; j++) {
            sum += b_seq(j);
        }
        for (idx_t j = 1; j < m; j++) {
            sum += c_seq(j);
        }
    } else {
        idx_t m = (n - 1) / 2;
        for (idx_t j = 1; j <= m; j++) {
            sum += b_seq(j) + c_seq(j);
        }
    }
    return sum;
}
