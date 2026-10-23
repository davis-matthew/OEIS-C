/*
 * A104424  Numbers n such that n, prime(n), prime(n)+n, prime(n)-n and
 *          prime(n)*n all lack the digit 4.
 * https://oeis.org/A104424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   has_no_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Numbers n such that n, prime(n), prime(n)+n, prime(n)-n and prime(n)*n all lack the digit 4. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_no_four.h"

/* ==========================================================
 *                         Prime sieve
 * ========================================================== */

static bool sieve_built = false;
static uint64_t *prime_list = NULL;   /* prime_list[1] = 2, prime_list[2] = 3, ... */
static size_t prime_count = 0;        /* number of primes stored (prime_list[prime_count] is the last) */
static const size_t SIEVE_LIMIT = 5000000;   /* 5 000 000 → ~348k primes */

/* Builds the sieve and fills prime_list.  Only done once. */
static void build_sieve(void)
{
    if (sieve_built) return;

    /* Boolean array: 1 = prime candidate, 0 = composite. */
    uint8_t *is_prime = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) return;               /* allocation failed – will abort on first use */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Allocate space for primes.  Estimate 400 000 slots – enough for >5 M. */
    size_t capacity = 400000;
    prime_list = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }
    prime_list[0] = 0;        /* dummy to make indices 1‑based */
    prime_count = 0;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            ++prime_count;
            if (prime_count >= capacity) {   /* grow dynamically */
                capacity *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, capacity * sizeof(uint64_t));
                if (!tmp) break;  /* allocation failed – will abort next call */
                prime_list = tmp;
            }
            prime_list[prime_count] = i;
        }
    }

    free(is_prime);
    sieve_built = true;
}

/* ==========================================================
 *                   Helpers
 * ========================================================== */

/* ==========================================================
 *          OEIS A104424: n-th qualifying integer
 * ========================================================== */

/* Note: The build system supplies seq_t and idx_t.  The function has
 * external linkage and returns the n‑th term (n ≥ 1).  Calls may
 * arrive in any order; the implementation is deterministic and
 * independent for each call. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for offset 0 */
    if (!sieve_built) build_sieve();
    if (!sieve_built) return (seq_t)0;  /* allocation failed */

    size_t found = 0;
    for (size_t i = 1; ; ++i) {
        if (i > prime_count) break;  /* safety – normally never reached */
        uint64_t p = prime_list[i];

        if (!has_no_four(i)) continue;
        if (!has_no_four(p)) continue;
        if (!has_no_four(p + i)) continue;
        if (!has_no_four(p - i)) continue;
        if (!has_no_four(p * i)) continue;

        ++found;
        if (found == (size_t)n) return (seq_t)i;
    }
    return (seq_t)0;   /* should never happen for valid n */
}
