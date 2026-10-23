/*
 * A107190  Primes of the form 2x^2 + 39y^2.
 * https://oeis.org/A107190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64, isqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: generate primes, test each for the representation
   p = 2*x^2 + 39*y^2.  The representation test checks all y up to
   sqrt(p/39) and verifies that (p - 39*y^2)/2 is a perfect square.  */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions provided by the build environment */
#include "../utils/is_prime_uint64.h"   /* bool is_prime_uint64(uint64_t) */
#include "../utils/isqrt_uint64.h"     /* uint64_t isqrt_uint64(uint64_t) */

/* Storage for found primes of the required form */
static seq_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static uint64_t last_checked = 1;   /* candidate to start from (2) */

/* Ensure capacity of the dynamic array */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(prime_list, new_cap * sizeof(seq_t));
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Append a new prime to the list */
static void add_prime(uint64_t val)
{
    ensure_prime_capacity(prime_cnt + 1);
    prime_list[prime_cnt++] = (seq_t)val;
}

/* Test whether a prime p has the representation 2*x^2 + 39*y^2 */
static bool has_representation(uint64_t p)
{
    uint64_t max_y = isqrt_uint64(p / 39);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t rem = p - 39 * y * y;   /* safe: 39*y*y <= p */
        if (rem & 1)  /* must be even to be 2*x^2 */
            continue;
        uint64_t half = rem >> 1;
        uint64_t s = isqrt_uint64(half);
        if (s * s == half)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; undefined for n<1 */

    /* If we already have enough terms, return the requested one */
    if (prime_cnt >= (size_t)n)
        return prime_list[n - 1];

    /* Generate primes until we reach n terms */
    for (;;) {
        ++last_checked;
        if (!is_prime_uint64(last_checked))
            continue;
        if (has_representation(last_checked)) {
            add_prime(last_checked);
            if (prime_cnt >= (size_t)n)
                return prime_list[n - 1];
        }
    }
}
