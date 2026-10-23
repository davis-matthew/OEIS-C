/*
 * A122932  a(n) = A000085(n) - A000079(n-1).
 * https://oeis.org/A122932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000085(n) - 2^(n-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of involutions on n symbols minus 2^(n-1) */
/* Uses the recurrence A000085(n) = A000085(n-1) + (n-1)*A000085(n-2) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

static seq_t *invol_cache = NULL;   /* cache of involution counts */
static size_t   invol_cap   = 0;     /* allocated size of the cache   */
static idx_t   invol_max    = 0;     /* highest n computed so far     */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 */
    if (n < 1) return (seq_t)0;

    /* Ensure the cache can hold values up to n */
    if ((size_t)n >= invol_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(invol_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* out of memory */
        invol_cache = tmp;
        invol_cap   = new_cap;
    }

    /* Initialise base cases if this is the first run */
    if (invol_max == 0) {
        invol_cache[0] = (seq_t)1;         /* A000085(0) = 1 */
        if (invol_cap > 1) invol_cache[1] = (seq_t)1;  /* A000085(1) = 1 */
        invol_max = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = invol_max + 1; i <= n; ++i) {
        seq_t prev1 = invol_cache[i - 1];
        seq_t prev2 = invol_cache[i - 2];
        invol_cache[i] = prev1 + (seq_t)(i - 1) * prev2;
    }
    invol_max = n;

    /* Return A000085(n) - 2^(n-1) */
    seq_t inv = invol_cache[n];
    seq_t pow = pow2(n - 1);   /* 2^(n-1) */
    return inv - pow;
}
