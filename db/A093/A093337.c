/*
 * A093337  Penultimate digits of the primes.
 * https://oeis.org/A093337
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = penultimate decimal digit of the (n‑6)-th prime for n ≥
 *            11
 * Range:     u8 n <= 115, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 200000U   /* enough for the 10 000th prime */
#define MAX_PRIMES 20000U

static bool primes_initialized = false;
static uint64_t primes_storage[MAX_PRIMES];
static size_t primes_found = 0U;

/* Lazy sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes_initialized) return;

    static unsigned char composite[PRIME_LIMIT + 1U];
    size_t i, j;

    composite[0] = 1U;
    composite[1] = 1U;

    for (i = 2U; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            if (primes_found < MAX_PRIMES) {
                primes_storage[primes_found++] = i;
            }
            if (i * i <= PRIME_LIMIT) {
                for (j = i * i; j <= PRIME_LIMIT; j += i) {
                    composite[j] = 1U;
                }
            }
        }
    }
    primes_initialized = true;
}

/* Return the penultimate decimal digit of the (n‑6)-th prime. */
seq_t a(idx_t n)
{
    if (n < 11U) return (seq_t)0U;   /* undefined for lower indices */

    idx_t idx = n - 6U;              /* 1‑based index of the desired prime */
    init_primes();

    if ((size_t)idx == 0U || (size_t)idx > primes_found)
        return (seq_t)0U;           /* safety guard for unexpected large n */

    uint64_t prime = primes_storage[(size_t)idx - 1U];
    uint64_t penultimate = (prime / 10U) % 10U;
    return (seq_t)penultimate;
}
