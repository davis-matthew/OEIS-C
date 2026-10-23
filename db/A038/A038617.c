/*
 * A038617  Primes not containing the digit '9'.
 * https://oeis.org/A038617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3597, u32 n <= 9294, u64 n <= 9294,
 *            u128 n <= 9294, big any n
 * Helpers:   has_digit9_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that do not contain the decimal digit 9. */
/* --------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_digit9_uint64.h"

/* Very small deterministic primality test for 64‑bit integers
   adequate for the ranges needed by this sequence. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/* --------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* lazy cache of generated terms */
    static seq_t *cache   = NULL;
    static size_t  len    = 0;
    static size_t  cap    = 0;
    static uint64_t next  = 3;   /* candidate after the prime 2 */
    static bool     started = false;

    if (n == 0)   /* sequence offset is 1, but return 0 for n<1 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (!started) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)2;    /* first prime */
        len     = 1;
        started = true;
    }

    /* generate primes until we have at least n terms */
    while (len < n) {
        if (!has_digit9_uint64(next) && is_prime_uint64(next)) {
            if (len == cap) {
                size_t new_cap = cap << 1;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;   /* reallocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)next;
        }
        next += 2;   /* test only odd numbers after 2 */
    }

    return cache[n - 1];
}
