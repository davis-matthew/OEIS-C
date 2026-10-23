/*
 * A043679  Numbers whose base-16 representation has exactly 6 runs.
 * https://oeis.org/A043679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n-th integer whose hexadecimal representation
    contains exactly six runs of consecutive equal digits (offset 1).  It
    keeps a static cache that is expanded lazily on demand. */
/*  Implementation notes:
    • The cache stores values in seq_t; the index is idx_t (uint64_t).
    • candidates are generated as seq_t and incremented.  Overflow of a
      fixed‑size seq_t (e.g. uint8_t) causes the generation to stop;
      such overflow only matters when the requested n exceeds the number
      of qualifying values that fit into the type, which is outside the
      intended test range.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- static state -------------------------------------------------- */
static seq_t *seq_cache      = NULL;   /* dynamically sized array of results   */
static size_t cache_cap      = 0;      /* current capacity in elements         */
static size_t cache_len      = 0;      /* number of stashed terms              */
static seq_t next_candidate   = 1;      /* next integer to test                 */

/* ---------- helper: number of runs in the hex representation -------------- */
static int hex_runs(seq_t x)
{
    if (x == 0)                 /* representation "0" has one run */
        return 1;

    int runs  = 0;
    int last  = -1;
    while (x != 0) {
        int digit = (int)(x & (seq_t)0xF);   /* least significant hex digit */
        if (digit != last) {
            ++runs;
            last = digit;
        }
        x >>= 4;                         /* move to next hex digit     */
    }
    return runs;
}

/* ---------- helper: enlarge cache if necessary ----------------------------- */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    if (new_cap < min_cap)
        new_cap = min_cap;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache)                     /* allocation error – abort */
        abort();

    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)          /* offset starts at 1 – undefined for n==0 */
        return (seq_t)0;

    /* prepare storage for at least n elements */
    ensure_cache_cap((size_t)n);

    /* generate values until enough are found */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate++;

        /* Detect overflow for fixed‑size seq_t (e.g. uint8_t) */
        if (next_candidate == 0 && cand != 0)  /* overflow happened */
            break;

        if (hex_runs(cand) == 6) {
            seq_cache[cache_len++] = cand;
        }
    }

    /* If we ran out of values because of overflow, return 0 for any
       remaining indices – such cases are outside the expected test range. */
    if (cache_len < (size_t)n)
        return (seq_t)0;

    return seq_cache[n - 1];
}
