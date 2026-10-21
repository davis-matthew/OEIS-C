/*
 * A098935  a(n) = p^7 - p^5 - p^3 - p^2 where p is prime(n).
 * https://oeis.org/A098935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^7 - p^5 - p^3 - p^2, where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Dynamic array of primes (all primes are < 2^64). */
static uint64_t *prime_cache = NULL;   /* pointer to the array            */
static size_t   prime_cnt   = 0;       /* number of primes currently      */
static size_t   prime_cap   = 0;       /* allocated capacity of the array */

/* Append a prime to the cache, resizing if necessary. */
static inline void prime_cache_append(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? (prime_cap << 1) : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                       /* OOM handling          */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
    prime_cache[prime_cnt++] = p;
}

/* Test whether x is prime, using only already found primes. */
static inline bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure we have at least n primes stored (1‑based). */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;     /* already enough primes */

    /* Start from the last known prime or from 2. */
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt - 1] + 1;
    if (candidate <= 2) candidate = 2;

    while (prime_cnt < (size_t)n) {
        if (candidate == 2) {          /* handle the first prime */
            prime_cache_append(2);
            if (prime_cnt >= (size_t)n) break;
            candidate = 3;
            continue;
        }
        if ((candidate & 1) == 0) {    /* skip even numbers */
            candidate++;
            continue;
        }

        if (is_prime_candidate(candidate))
            prime_cache_append(candidate);

        candidate += 2;                /* test next odd */
    }
}

/* Compute the n‑th term of OEIS A098935. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety: offset starts at 1 */

    ensure_primes(n);
    uint64_t p = prime_cache[n - 1];     /* nth prime (1‑based) */
    seq_t  pseq = (seq_t)p;

    seq_t p2 = pseq * pseq;              /* p^2 */
    seq_t p3 = p2 * pseq;                /* p^3 */
    seq_t p5 = p3 * p2;                  /* p^5 */
    seq_t p7 = p5 * p2;                  /* p^7 */

    return p7 - p5 - p3 - p2;            /* p^7 - p^5 - p^3 - p^2 */
}
