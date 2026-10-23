/*
 * A038960  Primes p such that 74 is not a square mod p.
 * https://oeis.org/A038960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 74 is not a quadratic residue mod p i.e.
 *            (74|p) = -1 for p prime. The sequence is stored lazily and
 *            generated on demand
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *prime_cache = NULL;   /* cache of found primes */
static size_t cache_len  = 0;       /* number of primes in cache   */
static size_t cache_cap  = 0;       /* allocated capacity          */
static idx_t   last_checked = 3;     /* next odd number to test     */

/* Append a new prime to the cache, enlarging it if necessary. */
static void append_prime(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
    prime_cache[cache_len++] = v;
}

/* Return true if 74 is a quadratic nonresidue modulo the odd prime p. */
static bool legendre_not_square(idx_t p)
{
    seq_t res = mod_pow((seq_t)74, (p - 1) / 2, (seq_t)p);
    return (res == (seq_t)p - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not used for n<1 */

    /* Grow the cache until it contains the n-th term. */
    while (cache_len < n) {
        if (is_prime(last_checked) && legendre_not_square(last_checked)) {
            append_prime((seq_t)last_checked);
        }
        /* advance to the next odd candidate */
        last_checked += 2;
    }
    return prime_cache[n - 1];
}
