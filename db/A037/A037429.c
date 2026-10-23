/*
 * A037429  Positive numbers having the same set of digits in base 5 and base
 *          6.
 * https://oeis.org/A037429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037429: positive numbers whose set of digits in base 5
   equals their set of digits in base 6 (computed by brute‑force search). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to test whether a value belongs to the sequence. */
static bool in_seq(seq_t x)
{
    seq_t temp = x;
    unsigned mask5 = 0;
    /* base 5: digits 0‑4 */
    while (temp > 0) {
        unsigned d = (unsigned)(temp % 5u);
        mask5 |= 1u << d;
        temp /= 5u;
    }

    temp = x;
    unsigned mask6 = 0;
    /* base 6: digits 0‑5, digit 5 cannot appear because base 5 never has it */
    while (temp > 0) {
        unsigned d = (unsigned)(temp % 6u);
        if (d == 5u)        /* digit 5 cannot be present in base 5 representation */
            return false;
        mask6 |= 1u << d;
        temp /= 6u;
    }

    return mask5 == mask6;
}

/* Static cache state */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of cached terms, 0‑based index */
static idx_t cache_cap  = 0;   /* allocated capacity */
static seq_t last_checked = 0; /* last tested integer */

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache until we have at least n terms. */
    while (cache_size < n) {
        last_checked++;
        if (in_seq(last_checked)) {
            /* Ensure capacity. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();           /* critical failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
}
