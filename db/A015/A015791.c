/*
 * A015791  a(n) is the smallest integer k such that phi(k) + n | sigma(k +
 *          n).
 * https://oeis.org/A015791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that phi(k)+n | sigma(k+n)
 * Range:     u8 n <= 32, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_uint64(uint64_t n) {
    seq_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            result -= result / (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

static seq_t sigma_uint64(uint64_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sum = 1;
            seq_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= (seq_t)p;
                sum += pow;
            }
            result *= sum;
        }
    }
    if (m > 1) {
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

seq_t a(idx_t n) {
    for (idx_t k = 1; ; ++k) {
        seq_t phi_k   = phi_uint64((uint64_t)k);
        seq_t sigma_kn = sigma_uint64((uint64_t)(k + n));
        seq_t denom   = phi_k + (seq_t)n;
        if (denom == 0) continue;      /* avoid division by zero, though impossible here */
        if (sigma_kn % denom == 0)
            return (seq_t)k;
    }
}
