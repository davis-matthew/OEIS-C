/*
 * A226040  a(n) = product{ p prime such that p divides n + 1 and p - 1 does
 *          not divide n }.
 * https://oeis.org/A226040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product{ p prime | p | (n+1) and (p-1) does not divide n
 *            }
 * Range:     u8 n <= 285, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Maximum prime needed for n up to about 200000 (n+1 <= 200001). Adjust if larger n are required. */
#define PRIME_LIMIT 200000

static uint32_t primes[PRIME_LIMIT + 1];
static size_t prime_count = 0;

/* Build prime table once */
static void init_primes(void) {
    if (prime_count != 0) return;   /* already initialized */

    size_t limit = PRIME_LIMIT;
    bool *sieve = (bool*)malloc(limit + 1);
    if (!sieve) return; /* allocation failure – unlikely in this context */

    /* 0 and 1 are not primes */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = false;

    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            primes[prime_count++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
}

/* Compute a(n) as defined */
seq_t a(idx_t n) {
    init_primes();

    uint64_t m = (uint64_t)n + 1ull;
    seq_t result = 1;

    if (m <= 1ull) return result;  /* for n = 0 */

    uint64_t temp = m;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            if (n % ((uint64_t)p - 1ull) != 0ull) {
                result *= (seq_t)p;
            }
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1ull) {   /* remaining prime factor > sqrt(m) */
        uint64_t p = temp;
        if (n % (p - 1ull) != 0ull) {
            result *= (seq_t)p;
        }
    }

    return result;
}
