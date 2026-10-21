/*
 * A079081  Numerator of
 *          (prime(n)+1)*(prime(n+1)+1)/(4*(prime(n)*prime(n+1)+1)).
 * https://oeis.org/A079081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   numerator of (prime(n)+1)*(prime(n+1)+1) /
 *            (4*(prime(n)*prime(n+1)+1))
 * Range:     u8 n <= 15, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses get_prime() from the utilities; computes using 64‑bit
 * intermediate arithmetic to avoid overflow on small seq_t types. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd64.h"

seq_t a(idx_t n) {
    /* retrieve consecutive primes (1‑based index) */
    uint64_t p = (uint64_t)get_prime(n);       /* prime(n)   */
    uint64_t q = (uint64_t)get_prime(n + 1);   /* prime(n+1) */

    /* calculate numerator and denominator in 64‑bit arithmetic */
    uint64_t numer = (p + 1) * (q + 1);                    /* (p+1)(q+1)  */
    uint64_t denom = 4 * (p * q + 1);                     /* 4(pq+1)     */

    /* reduce the fraction and cast back to seq_t */
    uint64_t g     = gcd64(numer, denom);
    return (seq_t)(numer / g);
}
