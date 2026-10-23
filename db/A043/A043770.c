/*
 * A043770  Number of runs in the base 2 representation of n is congruent to 7
 *          mod 10.
 * https://oeis.org/A043770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose binary representation
 *            contains a run count congruent to 7 (mod 10)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation maintains a static cache of the sequence values.
   New terms are found by scanning successive integers and recording those
   whose number of runs in base‑2 is 7 mod 10.  The cache grows only when
   needed, using the provided seq_realloc utility so it works with both
   C and C++ builds. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Dynamic cache of a(n) values. */
static seq_t *cache    = NULL;  /* array of seq_t */
static size_t cache_len = 0;    /* number of elements stored     */
static size_t cache_cap = 0;    /* current allocated capacity */

/* Path of the next integer to test.  This persists across calls. */
static idx_t next_candidate = 1;

/* Count the number of runs in the binary representation of x.
   Runs are counted as consecutive equal bits; the first bit starts the
   first run.  For example, x = 85 (0b1010101) has 7 runs. */
static int run_count(idx_t x)
{
    int runs = 0;
    int prev = -1;
    while (x != 0) {
        int bit = (int)(x & 1ULL);
        if (runs == 0 || bit != prev)
            ++runs;
        prev = bit;
        x >>= 1;
    }
    return runs;
}

/* Public sequence function.  It extends the cache until it contains the
   requested term and then returns it.  The function is safe for any
   order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but being defensive */
        return 0;

    /* Generate new terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* If the cache is full, grow it by a factor of 2 (or to n if
           that is larger). */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            if (new_cap < (size_t)n)
                new_cap = (size_t)n;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Check the next candidate.  If its run count is 7 mod 10, record
           it. */
        if (run_count(next_candidate) % 10 == 7) {
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
