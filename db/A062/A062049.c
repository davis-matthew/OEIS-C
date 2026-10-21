/*
 * A062049  Integer part of geometric mean of first n primes.
 * https://oeis.org/A062049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ (p₁·p₂·…·pₙ)^(1/n) ⌋ , where pᵢ is the i‑th prime
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/ensure_primes_upto.h"   /* ensure_primes_upto(idx_t n)          */
#include "../utils/get_prime.h"            /* uint32_t get_prime(idx_t k)          */

seq_t a(idx_t n)
{
    /* Prepare prime table up to n */
    ensure_primes_upto(n);

    /* Sum of logarithms of the first n primes (long double for high precision) */
    long double log_prod = 0.0L;
    for (idx_t i = 1; i <= n; ++i) {
        long double p = (long double)get_prime(i);
        log_prod += logl(p);          /* natural logarithm (base e) */
    }

    /* Binary search on the integer m satisfying m^n ≤ product < (m+1)^n */
    idx_t low = 1;
    idx_t high = (idx_t)get_prime(n);     /* pₙ > a(n) for all n ≥ 1 */

    /* Small epsilon to guard against floating‑point rounding */
    const long double eps = 1e-14L;

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        long double mid_log = logl((long double)mid);
        long double lhs = (long double)n * mid_log;
        if (lhs <= log_prod + eps)          /* mid^n ≤ product  */
            low = mid;
        else                                /* mid^n > product  */
            high = mid - 1;
    }

    return (seq_t)low;
}
