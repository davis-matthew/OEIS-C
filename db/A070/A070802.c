/*
 * A070802  a(n)=prevprime[sigma(n)]-nextprime[phi(n)]=A070801(n)-A070800(n).
 * https://oeis.org/A070802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prevprime[sigma(n)] - nextprime[phi(n)]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 77 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Largest prime <= m (m >= 2) */
static uint64_t prev_prime_of(uint64_t m)
{
    if (m < 2) return 0;          /* no prime ≤ 1 */
    while (m >= 2 && !is_prime_u64(m))
        --m;
    return m;
}

/* OEIS A070802 */
seq_t a(idx_t n)
{
    /* Works for n >= 2; caller guarantees offset */
    seq_t sigma_n = sigma_of(n);   /* sum of divisors */
    seq_t phi_n   = totient(n);        /* Euler's totient */

    uint64_t s = (uint64_t)sigma_n;
    uint64_t p = (uint64_t)phi_n;

    uint64_t prev = prev_prime_of(s);   /* prevprime[sigma(n)] */
    uint64_t next = next_prime(p);   /* nextprime[phi(n)] */

    return (seq_t)(prev - next);
}
