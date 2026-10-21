/*
 * A218850  a(n) is the least r > 1 for which the interval (r*(2*n-1),
 *          r*(2*n+1)) contains no prime, or 0 if no such r exists.
 * https://oeis.org/A218850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 8451, u64 n <= 8451,
 *            u128 n <= 8451, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A218850: least r>1 such that (r*(2*n-1), r*(2*n+1)) contains no prime, or 0 if none exists. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

#define SIEVE_LIMIT 20000000U   /* enough for all n <= 10000 and r <= 1000 */
#define R_MAX       1000U       /* upper bound for search of r */

static uint8_t prime_flags[SIEVE_LIMIT + 1];
static bool sieve_done = false;

/* Sieve of Eratosthenes to fill prime_flags: 1 => prime, 0 => composite */
static void init_sieve(void)
{
    if (sieve_done)
        return;

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        prime_flags[i] = 1;
    prime_flags[0] = prime_flags[1] = 0;

    for (size_t i = 2; (size_t)i * i <= SIEVE_LIMIT; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_flags[j] = 0;
        }
    }
    sieve_done = true;
}

/* Return the A218850 value for index n (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1, but safe to return 0 */

    init_sieve();

    seq_t N = (seq_t)n;
    seq_t twoN = 2 * N;

    for (seq_t r = 2; r <= R_MAX; ++r) {
        seq_t start = r * (twoN - 1) + 1;      /* first integer strictly > r*(2n-1) */
        seq_t end   = r * (twoN + 1) - 1;      /* last integer strictly < r*(2n+1) */

        bool has_prime = false;
        for (seq_t k = start; k <= end; ++k) {
            bool prime;
            if (k <= SIEVE_LIMIT) {
                prime = prime_flags[(size_t)k];
            } else {
                prime = is_prime_big(k);
            }
            if (prime) {
                has_prime = true;
                break;
            }
        }

        if (!has_prime)
            return r;      /* least r with a prime‑free interval */
    }

    return 0;              /* no such r found within R_MAX */
}
