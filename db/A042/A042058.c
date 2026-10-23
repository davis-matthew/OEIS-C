/*
 * A042058  Numerators of continued fraction convergents to sqrt(553).
 * https://oeis.org/A042058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042058: Numerators of continued‑fraction convergents to sqrt(553)
   Recurrence (classical continued‑fraction recurrence):
     p_{-2} = 0,  p_{-1} = 1,
     a_0   = 23,
     a_k   = periodic part of period 26 for k ≥ 1,
     p_n   = a_n * p_{n-1} + p_{n-2}   (n ≥ 0)
   The periodic part is
     [1, 1, 15, 5, 1, 4, 2, 1, 1, 3, 1, 2, 6, 2, 1,
      3, 1, 1, 2, 4, 1, 5, 15, 1, 1, 46]  (length 26).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Periodic part of the continued fraction of sqrt(553). */
static const seq_t PERIODIC[26] = {
    (seq_t)1, (seq_t)1, (seq_t)15, (seq_t)5,  (seq_t)1,
    (seq_t)4, (seq_t)2,  (seq_t)1, (seq_t)1, (seq_t)3,
    (seq_t)1, (seq_t)2,  (seq_t)6, (seq_t)2,  (seq_t)1,
    (seq_t)3, (seq_t)1,  (seq_t)1, (seq_t)2, (seq_t)4,
    (seq_t)1, (seq_t)5,  (seq_t)15, (seq_t)1, (seq_t)1,
    (seq_t)46
};

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool   initialized = false;
    static seq_t *cache       = NULL;   /* p_0, p_1, ... */
    static size_t capacity    = 0;
    static size_t length      = 0;      /* number of terms stored (index+1) */

    if (!initialized) {
        /* Allocate an initial capacity of 1 entry for p_0 */
        capacity = 1;
        cache = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        /* Failure in realloc is ignored; the caller will get 0 (invalid) */
        initialized = true;
    }

    /* Grow the cache if the requested index is outside current capacity */
    if (n >= capacity) {
        size_t newcap = capacity;
        while (n >= newcap)
            newcap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;     /* allocation failure: return 0 as safe value */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute new terms up to index n (if not already computed) */
    if (n >= length) {
        /* Prepare the recurrence state: p_{-1} = 1, p_{-2} = 0 */
        seq_t prev1, prev2;          /* prev1 = p_{i-1}, prev2 = p_{i-2} */

        if (length == 0) {
            /* No terms computed yet */
            prev1 = (seq_t)1;
            prev2 = (seq_t)0;
        } else if (length == 1) {
            /* Only p_0 is stored */
            prev1 = cache[0];      /* p_0 */
            prev2 = (seq_t)1;      /* p_{-1} */
        } else {
            /* Use the two most recent cached terms */
            prev1 = cache[length - 1];
            prev2 = cache[length - 2];
        }

        for (idx_t idx = (idx_t)length; idx <= n; ++idx) {
            seq_t ak;
            if (idx == 0) {
                /* The first partial quotient */
                ak = (seq_t)23;
            } else {
                /* Periodic partial quotients starting at a_1 */
                ak = PERIODIC[(size_t)(idx - 1) % 26];
            }
            /* p_i = a_i * p_{i-1} + p_{i-2} */
            seq_t pk = ak * prev1 + prev2;
            cache[(size_t)idx] = pk;
            /* Advance the recurrence buffers */
            prev2 = prev1;
            prev1 = pk;
        }
        length = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
