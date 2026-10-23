/*
 * A038949  Primes p such that 67 is a square mod p.
 * https://oeis.org/A038949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, jacobi_symbol, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th term of OEIS sequence A038949: primes p such that
 * 67 is a quadratic residue modulo p (i.e., Legendre(67|p)=1 or p=67).
 * The function caches qualifying primes and returns the n‑th one
 * (1‑based indexing as in the OEIS entry).  It works in any call order
 * and is efficient for values up to at least n=10000.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/seq_alloc.h"       /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"     /* seq_t *seq_realloc(seq_t*, size_t, size_t) */

static seq_t *prime_cache = NULL;  /* dynamically sized array of qualified primes */
static size_t cache_len  = 0;       /* number of primes currently stored */
static size_t cache_cap  = 0;       /* allocated capacity of prime_cache */

static uint64_t next_candidate = 3;  /* next odd number to test for primality */

/* Adjust the cache capacity to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;

    seq_t *p = seq_realloc(prime_cache, cache_cap, new_cap);
    prime_cache = p;
    cache_cap   = new_cap;
}

/* Return the n-th term (1‑based).  n==0 is undefined; return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Extend cache until it contains at least n qualifying primes. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;  /* advance to next odd candidate */

        /* Skip composite numbers. */
        if (!is_prime_big((seq_t)p))
            continue;

        bool ok = false;
        if (p == 67ULL) {
            ok = true;                  /* Legendre(67|67)=0, treated as square */
        } else {
            int ls = jacobi_symbol(67ULL, p); /* Legendre(67|p) */
            if (ls == 1) ok = true;
        }

        if (ok) {
            grow_cache(cache_len + 1);
            prime_cache[cache_len++] = (seq_t)p;
        }
    }

    return prime_cache[n - 1];
}
