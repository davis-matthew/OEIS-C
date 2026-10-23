/*
 * A142802  Primes congruent to 4 mod 61.
 * https://oeis.org/A142802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 61 == 4 (offset = 1)
 * Range:     u8 none, u16 n <= 107, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/alloc_seq_array.h"

static seq_t *primes = NULL;          /* cached primes */
static size_t primes_cap = 0;         /* capacity of the cache */
static size_t primes_cnt = 0;         /* number of primes stored */
static idx_t next_k = 0;              /* k such that 61*k+4 is the next candidate */

/* Ensure that at least n primes congruent to 4 (mod 61) are cached. */
static void
ensure_primes(idx_t n)
{
    /* Allocate or enlarge the storage when needed. */
    if (primes == NULL || primes_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < primes_cnt; ++i)
            tmp[i] = primes[i];
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Generate further primes until we reach the desired count. */
    while (primes_cnt < (size_t)n) {
        seq_t cand = (seq_t)61 * (seq_t)next_k + 4;
        if (is_prime((idx_t)cand)) {
            primes[primes_cnt++] = cand;
        }
        ++next_k;
    }
}

/* Return the nth prime congruent to 4 (mod 61). */
seq_t
a(idx_t n)
{
    if (n == 0)   /* this case never occurs for OEIS, but defined for safety */
        return 0;
    ensure_primes(n);
    return primes[n - 1];
}
