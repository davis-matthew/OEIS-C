/*
 * A107219  Primes of the form x^2 + 100y^2.
 * https://oeis.org/A107219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_uint64, isqrt_uint64, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form x^2 + 100*y^2 by direct search. */
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_uint64.h"   /* is_prime_uint64(uint64_t) */
#include "../utils/isqrt_uint64.h"     /* isqrt_uint64(uint64_t) */
#include "../utils/seq_malloc.h"       /* seq_malloc(idx_t) */
#include "../utils/seq_realloc.h"     /* seq_realloc(seq_t *, idx_t) */

static seq_t *cached = NULL;          /* array of found primes */
static size_t cached_capacity = 0;    /* allocated slots     */
static size_t cached_size    = 0;     /* number of stored primes */

/* Allocate or grow the cached array to at least `min` slots. */
static void ensure_capacity(size_t min)
{
    if (cached_capacity < min) {
        size_t new_cap = cached_capacity ? cached_capacity * 2 : 16;
        if (new_cap < min) new_cap = min;

        if (cached_capacity == 0) {
            cached = seq_malloc((idx_t)new_cap);
        } else {
            cached = seq_realloc(cached, (idx_t)new_cap);
        }
        cached_capacity = new_cap;
    }
}

/* Append the next prime of the required form to the cache. */
static void generate_next(void)
{
    static uint64_t candidate = 3;      /* next odd number to test */

    while (1) {
        /* Find the next odd prime >= candidate. */
        while (!is_prime_uint64(candidate)) {
            candidate += 2;
        }
        uint64_t p = candidate;
        candidate += 2;                /* next candidate after this prime */

        /* Test whether p can be written as x^2 + 100*y^2. */
        bool ok = false;
        uint64_t y_max = isqrt_uint64(p / 100);   /* maximal y with 100*y*y <= p */
        for (uint64_t y = 0; y <= y_max; ++y) {
            uint64_t term = 100ULL * y * y;
            uint64_t rem  = p - term;            /* p >= term */
            uint64_t s    = isqrt_uint64(rem);
            if (s * s == rem) {
                ok = true;
                break;
            }
        }
        if (ok) {
            /* Store the found prime. */
            ensure_capacity(cached_size + 1);
            cached[cached_size++] = (seq_t)p;
            return;
        }
        /* Continue searching for the next prime. */
    }
}

/* Fill the cache up to at least `n` terms. */
static void fill_to(idx_t n)
{
    while (cached_size < n)
        generate_next();
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n == 0; return 0 as a safe fallback. */
        return 0;
    }
    fill_to(n);
    return cached[n - 1];
}
