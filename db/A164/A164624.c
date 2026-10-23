/*
 * A164624  Primes p such that p + Floor[p/2] + Floor[p/3] is prime.
 * https://oeis.org/A164624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan primes p and keep those for which
   p + ⌊p/2⌋ + ⌊p/3⌋ is also prime.  The sequence is memoised
   across calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *good_cache = NULL;
static size_t good_len   = 0; /* number of entries stored */
static size_t good_cap   = 0; /* allocated capacity */

/* Last candidate examined (unsigned 64‑bit). */
static idx_t last_checked = 1;

/* Compute maximum value representable in seq_t, converted to idx_t. */
static const idx_t MAX_SEQ_T = (idx_t)(-(seq_t)1);

/* Ensure buffer can hold at least 'need' elements. */
static void
ensure_good_capacity(size_t need)
{
    if (need <= good_cap)
        return;
    size_t new_cap = good_cap ? good_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    /* In C++ a cast is required. */
    good_cache = (seq_t *)realloc(good_cache, new_cap * sizeof(seq_t));
    good_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset 1: a(1) is the first prime. */
        return 0;

    /* Generate terms until we have at least n. */
    while (good_len < (size_t)n)
    {
        /* Find the next prime satisfying the condition. */
        while (1)
        {
            const idx_t cand = ++last_checked;

            /* Stop if candidate cannot be stored in seq_t. */
            if (cand > MAX_SEQ_T)
                return 0;

            if (!is_prime_big((seq_t)cand))
                continue;

            /* Compute q = p + floor(p/2) + floor(p/3) in a larger type to
               avoid overflow before converting back to seq_t. */
            const idx_t q_idx = cand + cand / 2 + cand / 3;

            if (q_idx > MAX_SEQ_T)     /* q would overflow seq_t */
                continue;

            const seq_t q = (seq_t)q_idx;

            if (is_prime_big(q))
            {
                ensure_good_capacity(good_len + 1);
                good_cache[good_len++] = (seq_t)cand;
                break;   /* None the next sequence member. */
            }
        }
    }

    return good_cache[n - 1];
}
