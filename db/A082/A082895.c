/*
 * A082895  Closest number to sigma(n) = A000203(n) which is divisible by n.
 * https://oeis.org/A082895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n * floor( ( floor(n/2) + sigma(n) ) / n )
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all positive divisors of n. */

#include <stdint.h>

/* Compute sigma(n).  Result fits in 64‑bit for all n up to 10⁶. */
static uint64_t sigma_of(uint64_t n) {
    uint64_t sigma = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t term = 1, sum = 1;
            while (tmp % p == 0) {
                tmp /= p;
                term *= p;
                sum += term;
            }
            sigma *= sum;
        }
    }
    if (tmp > 1)            /* remaining prime factor */
        sigma *= (tmp + 1);
    return sigma;
}

seq_t a(idx_t n) {
    uint64_t sigma_u = sigma_of(n);
    seq_t n_seq   = (seq_t) n;
    seq_t n_half  = (seq_t)(n / 2);
    seq_t sigma   = (seq_t) sigma_u;
    seq_t total   = n_half + sigma;   /* numerator */
    seq_t k       = total / n_seq;    /* floor division */
    return n_seq * k;                 /* nearest multiple of n */
}
