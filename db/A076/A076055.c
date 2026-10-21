/*
 * A076055  Composite numbers which when read backwards are primes.
 * https://oeis.org/A076055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 5167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A076055: composite numbers m such that rev(m) is prime */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev.h"
#include "../utils/is_prime.h"

static seq_t *cache = NULL;      /* array of found terms */
static size_t      used = 0;     /* number of elements stored */
static size_t      capacity = 0; /* allocated capacity of cache */
static uint64_t    next = 2;     /* next integer to test */

/* Increase the capacity of the cache if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Allocation failed – terminate */
        abort();
    }
    cache = new_ptr;
    capacity = new_cap;
}

/* Return the nth term of A076055 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0, but never called for n < 1 */

    /* Generate terms until we have at least n of them */
    while (used < (size_t)n) {
        /* Skip numbers less than 4 – they're not composite */
        while (next < 4) ++next;

        uint64_t v = next++;
        if (is_prime(v))
            continue; /* v is prime, not composite */

        uint64_t rev_v = rev(v);
        if (is_prime(rev_v)) {
            /* v is composite and its reverse is prime */
            ensure_capacity(used + 1);
            cache[used++] = (seq_t)v;
        }
    }

    return cache[n - 1];
}
