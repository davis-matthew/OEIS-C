/*
 * A066102  Number of distinct prime factors of sigma_4(n), the sum of the 4th
 *          powers of divisors of n.
 * https://oeis.org/A066102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(sigma_4(n)) (number of distinct prime factors of
 *            σ₄(n))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_k.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safety for index 0, though not expected */

    /* Compute σ₄(n).  The result fits into 64 bits for n up to 10000. */
    seq_t sigma = sigma_k(n, 4);
    uint64_t sigma64 = (uint64_t)sigma;   /* cast for factorization */

    if (sigma64 <= 1) return 0;  /* σ₄(1) = 1 → no prime factors */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(sigma64, primes, exps, &cnt);
    return (seq_t)cnt;
}
