/*
 * A067887  Composite numbers k such that phi(k+1) < phi(k).
 * https://oeis.org/A067887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite integer k such that φ(k+1) < φ(k)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation performs a linear scan of composite numbers and
 * tests the totient inequality using a small static prime sieve.
 * It works for all supported seq_t sizes, including arbitrary‑precision
 * builds.                                      */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------- Prime sieve ----------------------------- */

/* Choose a sieve size large enough to factor any k we encounter when
 * searching for the first ~10 000 terms.  A bound of 200 000 guarantees
 * that sqrt(200 000) ≈ 447, which is more than sufficient.  The
 * algorithm is O(sieve_size log log sieve_size). */
#define SIEVE_LIMIT 200000

static uint32_t primes[SIEVE_LIMIT + 1];
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count != 0)
        return; /* already initialized */

    static bool sieve[SIEVE_LIMIT + 1];
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;
        }

    prime_count = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            primes[prime_count++] = (uint32_t)i;
}

/* ---------------------------- Totient -------------------------------- */

static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t tmp   = n;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (tmp > 1) {
        result = result / (seq_t)tmp * (seq_t)(tmp - 1);
    }
    return result;
}

/* ------------------------------ a(n) -------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n = 0, return 0 as a safe sentinel */
        return 0;

    init_primes();

    size_t found = 0;
    uint64_t k = 4; /* first composite candidate */

    while (found < n) {
        seq_t phi_k = phi_uint64(k);

        /* Composite test: φ(k) ≠ k - 1 holds for primes. */
        if (phi_k != (seq_t)(k - 1)) {
            seq_t phi_k1 = phi_uint64(k + 1);
            if (phi_k1 < phi_k) {   /* φ(k+1) < φ(k) */
                ++found;
                if (found == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }
    return 0; /* unreachable */
}
