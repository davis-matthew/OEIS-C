/*
 * A044865  Positive integers having the same number of base-9 runs of odd
 *          length as even.
 * https://oeis.org/A044865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   runs_equal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers, collecting those whose base‑9 digit blocks
   (runs) have the same number of odd‑length and even‑length runs.
   The n‑th such integer (offset 1) is returned. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/runs_equal.h"

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_cap = 0;         /* allocated capacity of cache   */
static size_t cache_count = 0;       /* number of values stored      */
static seq_t next_value = 1;         /* next integer to test         */

/* Ensure we have room for more cached values. */
static void ensure_cache_capacity(void)
{
    if (cache_count >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1 only; 0 is unused */

    while (cache_count < n) {
        if (runs_equal(next_value)) {
            ensure_cache_capacity();
            cache[cache_count++] = next_value;
        }
        next_value++;
    }
    return cache[(size_t)(n - 1)];
}
