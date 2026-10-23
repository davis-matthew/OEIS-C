/*
 * A043874  Numbers k such that the number of runs in the base-4
 *          representation of k is congruent to 7 (mod 10).
 * https://oeis.org/A043874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043874: a(n) is the n-th positive integer whose base‑4 representation
 * has the number of runs congruent to 7 (mod 10). */

/* included headers are required to handle integer types and dynamic memory */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define CACHE_CAP 20000   /* capacity is safely larger than 10000 */

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t next_candidate = 1;

/* count the number of runs in the base‑4 expansion of x.
 * runs = 1 + number of transitions between successive digits.
 */
static int runs_in_base4(seq_t x)
{
    int runs = 1;               /* at least one run */
    int prev = (int)(x % 4);    /* least significant digit */
    x /= 4;
    while (x != 0) {
        int d = (int)(x % 4);
        if (d != prev)
            ++runs;
        prev = d;
        x /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  The function is called for arbitrary n ≥ 1. */
    if ((size_t)n == 0)         /* safety, though the OEIS offset is 1 */
        return 0;

    /* generate until we have at least n entries */
    while (cache_len < (size_t)n) {
        /* if we exceed our preallocated cache, just stop */
        if (cache_len >= CACHE_CAP) {
            /* This should not happen for the intended limits. */
            return 0;
        }

        seq_t cand = next_candidate++;   /* candidate to test */
        int r = runs_in_base4(cand);
        if (r % 10 == 7) {               /* satisfies the condition */
            cache[cache_len++] = cand;
        }
    }

    /* return the n‑th term (array is 0‑indexed) */
    return cache[(size_t)n - 1];
}
