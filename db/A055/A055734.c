/*
 * A055734  Number of distinct primes dividing phi(n).
 * https://oeis.org/A055734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of φ(n) (n ≥ 1)
 * Range:     u8 n <= 522, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   calc_phi, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* In the OEIS definition n ≥ 1; if called with n < 1 we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Compute Euler's totient value φ(n). */
    seq_t phi = calc_phi((seq_t)n);

    /* Convert φ(n) to 64‑bit for prime‑divisor counting.  The value
       of φ(n) for n in the expression range is always < 2⁶⁴. */
    uint64_t phi64 = (uint64_t)phi;

    /* Count distinct prime divisors of φ(n). */
    int cnt = count_distinct_primes(phi64);

    return (seq_t)cnt;
}
