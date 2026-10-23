/*
 * A142004  Primes congruent to 28 mod 29.
 * https://oeis.org/A142004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 213, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 28 (mod 29) by testing successive candidates. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;     /* stored primes ≡ 28 (mod 29) */
static size_t primes_cap = 0;    /* allocated capacity */
static size_t primes_cnt = 0;    /* number of primes stored */

/* next candidate that is ≡ 28 (mod 29) */
static seq_t next_candidate = (seq_t)28;

/* generate primes until at least n of them are stored */
static void
_ensure_n(idx_t n)
{
    /* Grow array if needed */
    if (primes_cnt < n) {
        /* Allocate or enlarge storage */
        if (primes_cnt >= primes_cap) {
            size_t new_cap = primes_cap ? 2 * primes_cap : 4;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Generate candidates */
        while (primes_cnt < n) {
            if (is_prime_big(next_candidate)) {
                if (primes_cnt >= primes_cap) { /* Grow again if just reached capacity */
                    size_t new_cap = primes_cap ? 2 * primes_cap : 4;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_cnt++] = next_candidate;
            }
            next_candidate += (seq_t)29;   /* stay congruent to 28 (mod 29) */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but harmless */
    _ensure_n(n);
    return primes[n - 1];
}
