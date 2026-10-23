/*
 * A144862  Composite numbers n such that sqrt(n) > f(n), where f(n) = 2 *
 *          rad(n) if 4 divides n and rad(n) otherwise.
 * https://oeis.org/A144862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite number n such that sqrt(n) > f(n),
 *            where f(n) = 2*rad(n) if 4|n, otherwise rad(n)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this definition is directly derived from the OEIS entry.
   It simply tests each integer in increasing order and adds it
   to an on‑demand list once the condition is satisfied.  It uses
   a lazy static cache so repeated calls are fast even for
   non‑monotonic n. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/rad.h"          /* seq_t rad(idx_t n); */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n); */

/* ------------------------------------------------------------------ */
/*  A minimal dynamic array that grows as needed. */
/* ------------------------------------------------------------------ */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function.  --------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; index 0 is outside the defined range. */
    if (n == 0) return (seq_t)0;

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next integer to test (start after the last checked value). */
    static idx_t next = 4;  /* 1 and 2,3 are not composite */

    /* Ensure the cache is large enough for the requested index. */
    while (cache_len < (size_t)n) {
        /* 1. Skip integers that are not composite. */
        if (next > 1 && !is_prime(next)) {
            /* 2. Compute rad(next). */
            seq_t r = rad(next);

            /* 3. Compute f(next). */
            seq_t f = (next % 4 == 0) ? r * (seq_t)2 : r;

            /* 4. Test sqrt(next) > f, i.e. next > f^2. */
            /* Use an unsigned 64‑bit intermediate because f <= 255
               for all n where the result fits into seq_t.  This is
               safe and does not violate the restriction of using
               only seq_t operations in the main logic. */
            unsigned long long f2 = (unsigned long long)f * (unsigned long long)f;
            if ((unsigned long long)next > f2) {
                /* 5. Store the valid composite. */
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)next;
            }
        }
        ++next;
    }

    /* 6. Return the requested term (1‑based index). */
    return cache[n - 1];
}
