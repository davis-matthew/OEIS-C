/*
 * A133527  Sum of fifth powers of four consecutive primes.
 * https://oeis.org/A133527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^5 + p_{n+1}^5 + p_{n+2}^5 + p_{n+3}^5 * where p_k is
 *            the k‑th prime (1‑based). The sum is the OEIS * sequence A133527
 *            (sum of fifth powers of four consecutive primes)
 * Range:     u8 none, u16 n <= 1, u32 n <= 16, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fifth_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fifth_power.h"

#define MAX_PRIMES 20000        /* enough for n up to ~10000 */
#define SIEVE_LIMIT 200000      /* upper bound for sieve */

static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;
static bool primes_ready = false;

/* Build a small sieve and fill the primes array once.  The sieve limit
 * is generous enough for n up to about 10000. */
static void init_primes(void)
{
    if (primes_ready)
        return;

    bool *is_comp = (bool*)calloc(SIEVE_LIMIT + 1, 1);
    if (!is_comp)
        return;   /* allocation failed, leave primes empty */

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (uint64_t i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIMES; ++i) {
        if (!is_comp[i])
            primes[prime_count++] = (uint32_t)i;
    }
    free(is_comp);
    primes_ready = true;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, return 0 for index 0 (undefined) */
        return (seq_t)0;

    /* Ensure we have enough primes precomputed. */
    init_primes();

    /* Safety check: if not enough primes, return 0 (should not happen
     * for n <= 10000 with the chosen SIEVE_LIMIT). */
    if ((size_t)(n + 3) > prime_count)
        return (seq_t)0;

    seq_t sum = 0;
    for (int i = 0; i < 4; ++i) {
        uint32_t p = primes[(size_t)(n - 1) + i];
        sum += fifth_power(p);
    }
    return sum;
}
