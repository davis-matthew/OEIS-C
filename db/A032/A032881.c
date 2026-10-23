/*
 * A032881  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(0) > d(1) < d(2) > ...
 * https://oeis.org/A032881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate natural numbers and keep those whose decimal
   representation satisfies the alternating condition
   d0 > d1 < d2 > d3 < ... .
   The sequence is cached so that calls for arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>

/* Returns true iff all adjacent digits of x alternate,
   starting with x%10 > next digit. */
static bool is_alternating(seq_t x)
{
    if (x < 10) return true;          /* single digit always qualifies */

    seq_t prev = x % 10;
    x /= 10;
    bool greater = true;               /* next comparison must be prev > next */

    while (x > 0) {
        seq_t next = x % 10;
        if (greater) {
            if (!(prev > next)) return false;
        } else {
            if (!(prev < next)) return false;
        }
        greater = !greater;
        prev = next;
        x /= 10;
    }
    return true;
}

/* Cache of already computed terms. */
static seq_t *cached      = NULL;      /* array of terms */
static idx_t cache_size   = 0;          /* number of terms stored */
static size_t cache_cap   = 0;          /* allocated capacity */
static seq_t next_candidate = 1;        /* next number to test */

/* Grow the cache to at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < (size_t)needed) new_cap *= 2;
    seq_t *p = (seq_t*)realloc(cached, new_cap * sizeof(seq_t));
    cached = p;
    cache_cap = new_cap;
}

/* Return the n‑th term of A032881  (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined; guard against misuse */

    ensure_cache_capacity(n);

    while (cache_size < n) {
        /* Find the next number that satisfies the alternating condition. */
        while (!is_alternating(next_candidate))
            ++next_candidate;

        /* Store the found term. */
        cached[cache_size++] = next_candidate;

        /* Prepare the next candidate. */
        ++next_candidate;
    }

    return cached[n - 1];
}
