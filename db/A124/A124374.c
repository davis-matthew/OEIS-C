/*
 * A124374  Primes of the form !(k + 1)/2 = Sum_{i=0..k} i!/2.
 * https://oeis.org/A124374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of (∑_{i=0}^k i!)/2 where k ≥ 2, i.e. a(n) is
 *            the nth value of S(k)/2 that is prime
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static state to allow O(1) access to previously computed terms. */
static seq_t *primes = NULL;      /* array of previously found primes   */
static size_t primes_len = 0;      /* number of primes stored          */
static size_t primes_cap = 0;      /* current capacity of primes array  */
static idx_t   last_k  = 0;        /* largest k processed so far       */
static seq_t   fact    = 1;        /* fact  = k!  for current k        */
static seq_t   sum     = 1;        /* sum   = Σ_{i=0}^k i! for current k*/

/* Return the nth term of A124374, 1‑based.  When n is 0 or negative the
   function returns 0.  The function is resilient to calls in any order
   and grows its cache lazily until it contains at least n primes. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range indices */

    /* If we already know the nth prime, return it immediately. */
    if (primes_len >= (size_t)n)
        return primes[(size_t)n - 1];

    /* Continue generating primes until we reach the required index. */
    for (idx_t k = last_k + 1; ; ++k) {
        /* Update factorial: fact = k! */
        fact = fact * (seq_t)k;
        /* Update sum: sum = Σ_{i=0}^k i! */
        sum  = sum  + fact;

        if (k >= 2) {                   /* primes only start at k = 2  */
            seq_t candidate = sum / 2;
            if (is_prime_big(candidate)) {
                /* Grow the cache if necessary. */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *new_primes =
                        seq_realloc(primes, primes_cap, new_cap);
                    primes  = new_primes;
                    primes_cap = new_cap;
                }
                /* Store the prime and check whether we obtained n of them. */
                primes[primes_len++] = candidate;
                if (primes_len >= (size_t)n) {
                    last_k = k;                /* remember last processed k */
                    return primes[(size_t)n - 1];
                }
            }
        }
    }
}
