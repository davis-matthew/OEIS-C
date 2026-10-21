/*
 * A058525  Odd numbers z by which not all integers y, 0 <= y < 2^64, can be
 *          divided using "high multiplication" followed by a right shift.
 * https://oeis.org/A058525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   fails, log2_floor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058525 – odd integers z for which the high‑multiplication
 *  division trick fails for some 0 ≤ y < 2⁶⁴.
 *  The term a(n) is the (n+1)‑st such integer (a(0)=7, a(1)=21, …).
 *  We generate the sequence on demand and cache the results.
 *
 *  The algorithm:
 *  for an odd z, let e = ⌊log₂ z⌋.
 *  a = ceil(2^(64+e) / z).
 *  y = ⌊2^64 / z⌋·z – 1   (the special test value).
 *  If floor((a·y) / 2^(64+e)) ≠ floor(y / z)  then z fails.
 *
 *  Implementation notes:
 *  • 128‑bit integers (unsigned __int128) are used for intermediate
 *    calculations.  This is common in the OEIS C/C++ implementations.
 *  •  A simple dynamic array caches computed terms so subsequent
 *    calls are run‑time O(1).
 *  •  The code is fully C‑compatible (uses <stdint.h> and standard
 *    C‑style memory management).  It will compile with C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fails.h"
#include "../utils/log2_floor_uint64.h"

/* Forward declaration of build_cache */
static void build_cache(idx_t n_needed);

/* --------------------------------------------------------------- */
/*  Simple dynamic array cache for sequence terms                   */
/* --------------------------------------------------------------- */
static seq_t *cache    = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_z   = 3;  /* next odd z to test */

static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program you would handle the error; here we abort. */
        abort();
    }
    cache    = new_cache;
    cache_cap = new_cap;
}

static void build_cache(idx_t n_needed)
{
    /* Convert idx_t to size_t for array indexing.  idx_t is unsigned. */
    size_t wanted = (size_t)n_needed;
    while (cache_len <= wanted) {
        if (fails(next_z)) {
            /* Append the found term. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                grow_cache(new_cap);
            }
            cache[cache_len++] = (seq_t)next_z;
        }
        next_z += 2; /* test only odd z */
    }
}

/* --------------------------------------------------------------- */
/*  Public interface: returns the n‑th term of the sequence.      */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    build_cache(n);
    return cache[(size_t)n];
}
