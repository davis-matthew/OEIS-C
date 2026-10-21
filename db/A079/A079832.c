/*
 * A079832  Numbers k such that floor(reverse(k)/k) = 3.
 * https://oeis.org/A079832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079832 – Numbers k such that floor(reverse(k)/k) = 3.
   The function a(n) generates the nth term on demand, caching the
   generated values.  It evaluates the defining property
   floor(reverse(k)/k) == 3 by integer arithmetic. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* reverse the decimal digits of a seq_t value                           */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

/* ------------------------------------------------------------------ */
/* dynamic array for caching the terms.                                 */
static seq_t *cache      = NULL;   /* array of sequence values          */
static size_t cache_cap  = 0;      /* currently allocated capacity      */
static size_t cache_len  = 0;      /* number of stored terms            */
static seq_t last_checked_k = 1;   /* smallest k not yet tested         */

/* ------------------------------------------------------------------ */
/* grow the cache so that it can hold at least 'need' elements         */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
    if (new_cap < need)
        new_cap = need;

    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p) {
        /* allocation failed – abort so that the caller sees a failure */
        exit(EXIT_FAILURE);
    }
    cache = (seq_t *)p;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* public entry point – compute the nth term of A079832                */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold n terms. */
    ensure_cache_cap((size_t)n);

    /* Generate more terms until we have at least n cached. */
    while (cache_len < (size_t)n) {
        seq_t k   = last_checked_k;
        seq_t rev = reverse_digits(k);

        /* Test the defining property: floor(rev/k) == 3 */
        if (rev >= (seq_t)3 * k && rev < (seq_t)4 * k) {
            /* Store the term.  By construction cache_cap >= cache_len+1. */
            cache[cache_len++] = k;
        }
        last_checked_k = k + 1;
    }

    return cache[(size_t)n - 1];
}
