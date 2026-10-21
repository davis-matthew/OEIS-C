/*
 * A066674  Least number m such that phi(m) = A000010(m) is divisible by the
 *          n-th prime.
 * https://oeis.org/A066674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime m such that m ≡ 1 (mod pₙ), where pₙ is the
 *            n‑th prime. In other words, a(n) is the smallest prime ≡ 1 (mod
 *            (n‑th prime))
 * Range:     u8 n <= 10, u16 n <= 283, u32 n <= 572, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Cached prime list */
static uint64_t *s_primes = NULL;
static size_t    s_prime_count = 0;

/* Ensure that the cached list contains at least the first n primes. */
static void ensure_prime_limit(idx_t n)
{
    if (s_prime_count >= n) return;

    /* A crude upper bound for the nth prime:  n · 20 + 10
       suffices for n ≤ 10⁴ (the 10 000‑th prime is 104 729). */
    idx_t limit = n * 20 + 10;
    if (limit < 100) limit = 100;          /* safety for very small n */

    /* Rebuild the prime list.  Free the old data if present. */
    free(s_primes);
    s_primes = NULL;
    s_prime_count = 0;
    build_primes_upto(limit, &s_primes, &s_prime_count);
}

/* --- Public function required by the task --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 – undefined for n < 1 */

    ensure_prime_limit(n);               /* guarantee pₙ is available */

    uint64_t p = s_primes[n - 1];         /* the n‑th prime (1‑based) */
    seq_t   mod = (seq_t)p;               /* convert to seq_t for arithmetic */

    idx_t k = 1;                          /* multiplier in m = k·p + 1 */
    for (;;) {
        seq_t m = (seq_t)k * mod + 1;     /* candidate m */
        if (is_prime_big(m))
            return m;                    /* found the least prime ≡ 1 (mod pₙ) */
        ++k;
    }
}
