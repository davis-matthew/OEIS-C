/*
 * A000441  a(n) = Sum_{k=1..n-1} k*sigma(k)*sigma(n-k).
 * https://oeis.org/A000441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/24 - n^2/4)*sigma_1(n) + (5*n/24)*sigma_3(n)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 239, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 970 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma1(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum += (seq_t)i;
            if (i != n / i) {
                sum += (seq_t)(n / i);
            }
        }
    }
    return sum;
}

static seq_t sigma3(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            seq_t i_seq = (seq_t)i;
            sum += i_seq * i_seq * i_seq;
            if (i != n / i) {
                idx_t j = n / i;
                seq_t j_seq = (seq_t)j;
                sum += j_seq * j_seq * j_seq;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t s1 = sigma1(n);
    seq_t s3 = sigma3(n);
    
    seq_t term1 = 5 * s3;
    seq_t term2 = s1 * (6 * n_seq - 1);
    
    seq_t numerator = n_seq * (term1 - term2);
    
    return numerator / 24;
}
