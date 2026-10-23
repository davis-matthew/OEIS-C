/*
 * A142687  Primes congruent to 35 mod 57.
 * https://oeis.org/A142687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p % 57 == 35 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *primes = NULL;          /* cached primes */
static size_t primes_len = 0;         /* number of cached primes */
static size_t primes_cap = 0;         /* allocated capacity of the cache */
static idx_t candidate = 35;          /* first number to test will become 92,149,… */

static void add_prime(seq_t p)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure aborts */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

static void ensure_count(idx_t n)
{
    while (primes_len < n) {
        candidate += 57;           /* next candidate ≡ 35 (mod 57) */
        if (is_prime(candidate))   /* primality test */
            add_prime((seq_t)candidate);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below the offset */
    ensure_count(n);
    return primes[(size_t)(n - 1)];
}
