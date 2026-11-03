/*
 * A154865  Numbers k such that smallest digit of k is prime and the smallest
 *          digit of prime(k) is also prime.
 * https://oeis.org/A154865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A154865
   The function searches for integers k where the smallest decimal digit of
   k is prime (2,3,5,7) and the smallest decimal digit of the k‑th prime number
   is also prime.  It stores the results in a static array, expanding the
   array lazily so that the function works for random access or increasing
   indices. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Smallest decimal digit of a value that fits in idx_t (typically uint64_t). */
static int min_digit_idx_t(idx_t x) {
    if (x == 0) return 0;
    int mn = 9;
    while (x) {
        int d = (int)(x % 10);
        if (d < mn) mn = d;
        if (mn == 0) break;     /* cannot get smaller */
        x /= 10;
    }
    return mn;
}

/* Smallest decimal digit of a 32‑bit integer (used for the k‑th prime). */
static int min_digit_uint32(uint32_t x) {
    if (x == 0) return 0;
    int mn = 9;
    while (x) {
        int d = (int)(x % 10);
        if (d < mn) mn = d;
        if (mn == 0) break;
        x /= 10;
    }
    return mn;
}

/* Test if a digit is one of the prime digits 2,3,5,7. */
static bool prime_digit(int d) {
    return d == 2 || d == 3 || d == 5 || d == 7;
}

/* Static cache for the sequence values. */
static seq_t *cache  = NULL;
static idx_t cache_len = 0;   /* capacity of cache */
static idx_t found     = 0;   /* number of terms already found */
static idx_t next_k    = 1;   /* next k to test */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Expand the cache if necessary. */
    if (n > cache_len) {
        seq_t *new_cache = mem_resize(seq_t, cache, n);
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_len = n;
    }

    /* Generate terms until we have at least n of them. */
    while (found < n) {
        /* Check conditions for current k. */
        if (prime_digit(min_digit_idx_t(next_k))) {
            uint32_t pk = get_prime(next_k);   /* k-th prime */
            if (prime_digit(min_digit_uint32(pk))) {
                cache[found] = (seq_t)next_k;   /* store as seq_t */
                ++found;
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}
