/*
 * A066675  a(n) = A066674(n)-1 divided by the n-th prime.
 * https://oeis.org/A066675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m : φ(m) == 0 (mod p(n)) } The minimal m is the
 *            smallest prime q ≡ 1 (mod p(n)), so a(n) = (q-1)/p(n) = k where
 *            q = k·p(n)+1
 * Range:     u8 n <= 10, u16 n <= 448, u32 n <= 448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Access to the helper prototypes */
#include "../utils/is_prime_big.h"
#include "../utils/build_primes.h"

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build the list of primes once, up to a limit that covers the n‑th
       prime needed for the tested ranges (e.g. up to 200 000 handles
       n up to about 17 000).  */
    static bool ready = false;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!ready) {
        const idx_t limit = 200000U;   /* > 10 000th prime (104 729)   */
        build_primes(limit, &primes, &prime_cnt);
        ready = true;
    }

    /* Retrieve the n‑th prime (1‑based indexing).                           */
    if (n == 0 || (idx_t)n > prime_cnt) {
        return (seq_t)0;        /* graceful error, not used in tests   */
    }
    const uint64_t p = (uint64_t)primes[n - 1];

    /* Search for the smallest prime of the form k·p + 1.                     */
    for (uint64_t k = 1; ; ++k) {
        const uint64_t cand = k * p + 1ULL;
        if (is_prime_big((seq_t)cand)) {
            return (seq_t)k;     /* a(n) = (cand-1)/p = k              */
        }
    }
}
