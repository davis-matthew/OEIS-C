/*
 * A176762  Smallest positive integer that takes n steps to reach a cycle
 *          under iteration of the sum-of-squares-of-digits map A003132.
 * https://oeis.org/A176762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176762: Smallest positive integer that takes n steps to
   reach a cycle under iteration of the sum‑of‑squares‑of‑digits map
   (A003132).  This implementation explores numbers sequentially,
   keeps the first (hence smallest) occurrence for every pre‑period
   length, and returns the stored result when it is known.

   The function works for any integer type supplied for seq_t and
   idx_t.  It uses a static cache that is grown as needed and is
   independent of the order of calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_square_sum.h"

/* static cache storing the minimal integer that yields a given
   pre‑period length.  Index 0 is the fixed point 0, which is not
   relevant here; we use it to hold the value 1 (a(0)).  */
static seq_t *min_steps = NULL;   /* min_steps[steps] = minimal integer */
static size_t   min_cap   = 0;    /* allocated capacity of min_steps     */
static uint64_t next_x    = 2;    /* next integer to test (1 already stored) */
static uint64_t max_filled = 0;   /* largest step index that has a stored value */

/* Upper bound for a sum‑of‑squares‑of‑digits value on 64‑bit integers.
   If a value exceeds this bound we do not track it with the seen
   array; all cycle elements are small, so this is sufficient. */
#define VIS_MAX 2000
static unsigned int seen[VIS_MAX + 1] = {0};
static unsigned int mark = 1; /* time stamp for marking */

// ------------------------------------------------------------------
// Grow min_steps array if necessary.  New cells are zeroed (meaning
// “not yet set”).
static void ensure_cap(size_t need)
{
    if (need < min_cap)
        return;

    size_t new_cap = min_cap ? (min_cap << 1) : 32;
    while (new_cap <= need)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = static_cast<seq_t*>(realloc(min_steps, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t*)realloc(min_steps, new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        abort();
    memset(tmp + min_cap, 0, (new_cap - min_cap) * sizeof(seq_t));
    min_steps = tmp;
    min_cap   = new_cap;
}

// ------------------------------------------------------------------
// Count how many steps (pre‑period length) the integer `x` takes until
// a value repeats.  The cycle is reached when the same value occurs
// twice (including the start value).
static unsigned int preperiod_len(uint64_t x)
{
    unsigned int steps = 0;
    uint64_t cur = x;

    /* Use a fresh marking stamp for this candidate. */
    ++mark;
    if (mark == 0) {          /* overflow unlikely with 32‑bit */
        mark = 1;
        memset(seen, 0, sizeof(seen));
    }

    if (cur <= VIS_MAX)
        seen[(int)cur] = mark;

    while (1) {
        uint64_t next = digit_square_sum(cur);
        if (next <= VIS_MAX && seen[(int)next] == mark)
            break;                /* cycle reached */

        if (next <= VIS_MAX)
            seen[(int)next] = mark;
        ++steps;
        cur = next;
    }
    return steps;
}

// ------------------------------------------------------------------
seq_t a(idx_t n)
{
    /* Initialise static state only once.  a(0) = 1. */
    if (min_steps == NULL) {
        ensure_cap(1);
        min_steps[0] = (seq_t)1;
        max_filled   = 0;
        next_x       = 2;
    }

    if (n == 0)          /* a(0) = 1 */
        return (seq_t)1;

    /* Search until we have found the minimal integer with pre‑period
       length exactly n.  We always keep the cache for any steps
       discovered up to the current maximum. */
    while (min_steps[n] == (seq_t)0) {
        uint64_t cand = next_x++;
        unsigned int steps = preperiod_len(cand);
        ensure_cap(steps);

        if (min_steps[steps] == (seq_t)0)
            min_steps[steps] = (seq_t)cand;

        if (steps > max_filled)
            max_filled = steps;
    }

    return min_steps[n];
}
