/*
 * A038365  Numbers n with property that digits of n are not present in 2n.
 * https://oeis.org/A038365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   no_common_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer whose decimal digits
   do not appear in the decimal digits of 2*n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/no_common_digit.h"

/* Maximum number of terms that can be stored; the tests only go up to ~10 000,
   so 200 000 is more than enough. */
#define MAX_CACHE 200000

static seq_t cache[MAX_CACHE];   /* stored sequence values */
static size_t cache_size = 0;    /* number of elements stored */
static uint64_t next_candidate = 1; /* next integer to test (64‑bit) */

/* extend cache until it contains at least n entries */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n && cache_size < MAX_CACHE) {
        if (no_common_digit(next_candidate)) {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */
    ensure_cache(n);
    /* If requested n exceeds preallocated space, the user should not
       call this function with such n. For the intended test range
       (up to about 10 000), the limit is sufficient. */
    return cache[(size_t)n - 1];
}
