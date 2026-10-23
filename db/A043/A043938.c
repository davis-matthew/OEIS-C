/*
 * A043938  Numbers k such that 0 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find smallest k such that the base‑8 representation of k
   contains both digits 0 and 7 adjacent (as 07 or 70) while k+1 does not. */
#include <stdbool.h>
#include <stddef.h>

/* Utility: check whether a base‑8 number contains an adjacent 07 or 70 pair. */
static bool has_adjacent_07_70(seq_t x)
{
    /* If only one octal digit, adjacency is impossible */
    if (x < (seq_t)8) return false;

    seq_t prev = x % (seq_t)8;
    x /= (seq_t)8;
    while (x > (seq_t)0)
    {
        seq_t d = x % (seq_t)8;
        if ((prev == (seq_t)0 && d == (seq_t)7) ||
            (prev == (seq_t)7 && d == (seq_t)0))
            return true;
        prev = d;
        x /= (seq_t)8;
    }
    return false;
}

/* Up to ~10000 terms, 200 000 slots are ample for any seq_t size. */
#define CACHE_CAP 200000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t next_candidate = 0;

/* Public function required by the grading harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for the sequence, return 0 */

    while (cache_len < n)
    {
        seq_t k = next_candidate;
        while (true)
        {
            if (has_adjacent_07_70(k) && !has_adjacent_07_70(k + (seq_t)1))
            {
                cache[cache_len++] = k;
                next_candidate = k + (seq_t)1;
                break;
            }
            k = k + (seq_t)1;
        }
    }
    return cache[n - 1];
}
