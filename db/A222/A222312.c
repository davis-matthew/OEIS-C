/*
 * A222312  a(n) = n + A001222(n) - 1.
 * https://oeis.org/A222312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Ω(n) - 1 (Ω(n) = number of prime factors counted with
 *            multiplicity)
 * Range:     u8 n <= 251, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

/* Compute A222312: a(n) = n + number_of_prime_factors(n) - 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used by OEIS offset, but safe */
    uint64_t m = (uint64_t)n;      /* work in 64‑bit for factorisation */
    int cnt = 0;                   /* counts prime factors with multiplicity */

    /* factor out powers of 2 */
    while ((m & 1ULL) == 0) {
        ++cnt;
        m >>= 1;
    }

    /* trial division by odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {  /* p*p <= m  but avoid overflow */
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
    }

    /* if m > 1 now, it is a prime factor greater than sqrt(original n) */
    if (m > 1) ++cnt;

    return (seq_t)n + (seq_t)cnt - (seq_t)1;
}
