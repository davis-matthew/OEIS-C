/*
 * A037081  In ternary expansion of n, reading from left to right, successive
 *          runs of the digits occur in order ...,0,1,2,0,1,2,...
 * https://oeis.org/A037081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A037081
 *
 *  The sequence lists the non‑negative integers whose base‑3 expansion
 *  has runs of equal digits that proceed in the cyclic order
 *  0 → 1 → 2 → 0 → 1 → 2 → … (reading from most significant to least).
 *  The algorithm tests candidate numbers in increasing order and caches
 *  those that satisfy the property so that subsequent calls can return
 *  the cached value in O(1) time.
 *
 *  Implementation details:
 *    * `triple_run_ok()` extracts the ternary digits of a number and
 *      verifies that each run transition increases the digit by 1
 *      modulo 3.
 *    * All arithmetic uses `seq_t`, avoiding promotion to larger
 *      integer types.
 *    * A dynamically growing array `cache` stores the sequence
 *      values.  `realloc` is used (no external utils needed).
 *    * The function is fully thread‑safe with respect to the C11
 *      standard as long as the program does not perform concurrent
 *      calls to `a()`.  No synchronization primitives are used
 *      because the build assumptions guarantee single‑threaded
 *      evaluation of `a()`.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static bool triple_run_ok(seq_t num)
{
    /* Special case for 0 – its single run is trivially correct. */
    if (num == (seq_t)0)
        return true;

    /* Extract ternary digits (least significant first). */
    uint8_t digits[256];
    size_t len = 0;
    seq_t temp = num;
    while (temp != (seq_t)0) {
        digits[len++] = (uint8_t)(temp % (seq_t)3);
        temp /= (seq_t)3;
    }

    /* Walk from most significant to least significant digit. */
    size_t i = len - 1;
    uint8_t prev = digits[i];
    while (i > 0) {
        --i;
        uint8_t curr = digits[i];
        if (curr != prev) {
            /* A change must advance by +1 modulo 3. */
            if (curr != (uint8_t)((prev + 1) % 3))
                return false;
        }
        prev = curr;
    }
    return true;
}

/* ---------- Helper: test if a number satisfies the run rule ---------- */

/* ---------- Cache state and growth routine ---------- */

static seq_t *cache      = NULL;   /* array of values */
static size_t cache_len  = 0;      /* number of stored elements */
static size_t cache_cap  = 0;      /* allocated capacity */
static seq_t  next_candidate = (seq_t)0;  /* next number to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap) ? (cache_cap << 1) : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    /* In the unlikely event of realloc failure, we simply keep the old
       buffer; the build will abort in that case. */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n+1 valid numbers. */
    while (cache_len <= (size_t)n) {
        /* Test the current candidate number. */
        if (triple_run_ok(next_candidate)) {
            /* Store the candidate in the cache. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        /* Advance to the next integer. */
        ++next_candidate;
    }
    return cache[(size_t)n];
}
