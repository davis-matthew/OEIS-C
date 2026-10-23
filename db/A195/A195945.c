/*
 * A195945  Powers of 13 which have no zero in their decimal expansion.
 * https://oeis.org/A195945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth power of 13 whose decimal representation contains
 *            no digit 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   grow_seq, no_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/no_zero_digit.h"

/* Cache of sequence terms (0‑indexed).  cache_len holds the number of valid terms stored. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Current power of 13 being examined (13^k, starting with k=0). */
static seq_t pow_next          = (seq_t)1;   /* 13^0 at first use */
static bool pow_next_initialized = false;

/* Return nth term of the sequence (n >= 1).  n==0 is undefined, returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset guard */

    if (!pow_next_initialized) {                /* initialise on first call */
        pow_next          = (seq_t)1;          /* 13^0 */
        pow_next_initialized = true;
    }

    /* Fill the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (no_zero_digit(pow_next)) {
            /* ensure space for a new term */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = pow_next;
        }
        /* move to next power of 13 */
        pow_next = pow_next * (seq_t)13;
    }

    return cache[(size_t)n - 1];
}
