/*
 * A137351  Composite numbers n such that x^2 - n*y^2 represents -1.
 * https://oeis.org/A137351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4455, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137351:
   Composite numbers n such that x^2 - n*y^2 = -1 has integer solutions.
   The condition holds iff the continued‑fraction period of sqrt(n) is odd.
   We generate the terms by testing successive integer values, skipping
   perfect squares and primes, and keeping those with an odd period. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t *terms = NULL;      /* cached terms */
static size_t terms_size = 0;    /* number of cached terms */
static size_t terms_cap  = 0;    /* allocated capacity */
static idx_t next_candidate = 2; /* integer to test next */

/* Test whether n (uint64) is composite.
   (1, 2, 3 are not composite.) */
static bool is_composite_u64(idx_t n)
{
    if (n < 4) return false;
    if ((n & 1ULL) == 0ULL) return n > 2;
    idx_t lim = _floor_isqrt_u64(n);
    for (idx_t d = 3; d <= lim; d += 2)
        if (n % d == 0) return true;
    return false;
}

/* Return true iff the continued‑fraction period of sqrt(D) is odd.
   Perfect squares are excluded. */
static bool period_is_odd_u64(idx_t D)
{
    idx_t a0 = _floor_isqrt_u64(D);
    if ((unsigned __int128)a0 * a0 == D) return false; /* skip squares */

    idx_t m = 0, d = 1, a = a0;
    idx_t period = 0;

    do {
        /* m = d*a - m; */
        unsigned __int128 tmp = (unsigned __int128)d * a - m;
        m = (idx_t)tmp;

        /* d = (D - m*m) / d; */
        unsigned __int128 rem = (unsigned __int128)D - (unsigned __int128)m * m;
        d = (idx_t)(rem / d);

        /* a = (a0 + m) / d; */
        a = (a0 + m) / d;

        period++;
    } while (a != 2 * a0);

    return (period & 1U) == 1U;
}

/* Ensure the terms array has space for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) { /* allocation failed */
        fprintf(stderr, "allocation failure\n");
        abort();
    }
    terms = new_arr;
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but safe return */

    /* Fill cache up to needed index */
    while (terms_size < n) {
        /* Search for next composite with odd period */
        while (true) {
            if (is_composite_u64(next_candidate) && period_is_odd_u64(next_candidate))
                break;
            ++next_candidate;
        }
        ensure_capacity(terms_size + 1);
        terms[terms_size] = (seq_t)next_candidate;
        ++terms_size;
        ++next_candidate;
    }
    return terms[n - 1];
}
