/*
 * A173137  n-th nonnegative noncomposite number plus n.
 * https://oeis.org/A173137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A158611(n) + n (A158611 = nonnegative non‑composite
 *            numbers)
 * Range:     u8 n <= 48, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* local prime cache for computing A158611(n) */
static uint64_t *primes = NULL;        /* array of primes */
static size_t primes_count = 0;        /* number of primes stored */
static size_t primes_cap = 0;          /* allocated capacity */
static uint64_t next_candidate = 2;    /* next integer to test for primality */

/* ensure that at least 'need' primes are stored in the cache */
static void ensure_primes_count(idx_t need)
{
    /* nothing to do for need==0 */
    if (need == 0) return;

    /* grow primes array as needed */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
    }

    while (primes_count < (size_t)need) {
        /* test next_candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store the prime */
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 0 + 1 = 1, a(2) = 1 + 2 = 3, a(n) = prime_{n-2} + n for n >= 3 */
    if (n < 1) return (seq_t)0;        /* undefined for n < 1, return 0 */

    seq_t base;
    if (n == 1) {
        base = (seq_t)0;
    } else if (n == 2) {
        base = (seq_t)1;
    } else {
        ensure_primes_count(n - 2);    /* need at least (n-2) primes */
        base = (seq_t)primes[n - 3];   /* prime index: primes[0] = 2 is the first prime */
    }
    return base + (seq_t)n;
}
