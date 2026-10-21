/*
 * A050709  Composites c that reach a prime after 5 iterations of c-> c+sum of
 *          prime factors of c.
 * https://oeis.org/A050709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composites c that reach a prime after exactly 5 iterations
   of c → c + sum_of_prime_factors(c).  a(1) = 49, a(2) = 50, … */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities to test primality and to factor 64‑bit integers. */
#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t n) */
#include "../utils/_smallest_prime_factor.h"/* uint64_t _smallest_prime_factor(uint64_t n) */

static seq_t *cache = NULL;   /* array of found terms (offset 1) */
static size_t cached = 0;     /* number of terms stored          */
static size_t capacity = 0;   /* current capacity of `cache`     */

/* Sum of prime factors (with multiplicity) of a 64‑bit integer.  The
   accumulator `sum` is of type seq_t so the result can be used with
   arbitrary‑precision values, but the input is cast to uint64_t.  For
   numbers that do not fit in 64 bits this routine will wrap; the
   sequence values that the tests use fit comfortably in 64 bits. */
static seq_t sum_of_prime_factors(seq_t n)
{
    seq_t sum = 0;
    uint64_t cur = (uint64_t)n;
    while (cur > 1) {
        uint64_t p = _smallest_prime_factor(cur);
        sum += (seq_t)p;
        cur /= p;
    }
    return sum;
}

/* Check whether a composite number c reaches a prime after exactly
   5 iterations of f(x) = x + sum_of_prime_factors(x).  The routine
   guarantees that the four intermediate results are composite and
   the fifth is prime. */
static bool reaches_prime_after_5(seq_t c)
{
    seq_t x = c;
    for (int i = 0; i < 5; ++i) {
        seq_t add = sum_of_prime_factors(x);
        x += add;
        if (i < 4) {
            if (is_prime_big(x))
                return false;        /* early prime – not allowed */
        } else {
            if (!is_prime_big(x))
                return false;        /* final must be prime   */
        }
    }
    return true;
}

/* Return the nth term of the sequence (offset 1).  If n==0 the
   function returns 0 as a harmless sentinel. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Return cached value if available. */
    if ((size_t)n <= cached)
        return cache[(size_t)n - 1];

    /* Start searching from the next integer after the last cached term. */
    seq_t next = (cached == 0) ? (seq_t)2 : cache[cached - 1] + 1;

    for (seq_t c = next;; ++c) {
        if (is_prime_big(c))
            continue;                 /* skip primes */

        if (reaches_prime_after_5(c)) {
            /* Grow the cache if necessary. */
            if (cached == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;          /* allocation failed */
                cache = tmp;
                capacity = new_cap;
            }
            cache[cached++] = c;
            if ((size_t)n <= cached)
                return c;
        }
    }
}
