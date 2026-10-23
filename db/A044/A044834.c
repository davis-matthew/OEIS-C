/*
 * A044834  Positive integers having more base-8 runs of even length than odd.
 * https://oeis.org/A044834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044834.
   The function searches integers in increasing order and selects
   those whose base‑8 representation has more runs of even length
   than runs of odd length. It memoises found terms to answer
   repeated calls efficiently. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t cache[200000];
static size_t cache_len = 0;
static const size_t cache_cap = 200000;
static seq_t last_tested = 0;

/* Return true if x has more even‑length runs than odd‑length runs
   in its base‑8 representation. */
static bool qualifies(seq_t x)
{
    if (x == 0) return false;
    seq_t y = x;
    unsigned prev_digit = 255;   /* sentinel: no previous digit yet */
    seq_t run_len = 0;
    seq_t evens = 0, odds = 0;

    while (y > 0) {
        unsigned digit = (unsigned)(y % 8);
        y /= 8;
        if (digit == prev_digit) {
            run_len++;
        } else {
            if (prev_digit != 255) {
                if (run_len & 1) odds++; else evens++;
            }
            prev_digit = digit;
            run_len = 1;
        }
    }
    if (prev_digit != 255) {
        if (run_len & 1) odds++; else evens++;
    }
    return evens > odds;
}

/* Append a new value into the cache.  Capacity is fixed; the
   program aborts if the limit is exceeded. */
static void append_cache(seq_t val)
{
    if (cache_len >= cache_cap) abort();
    cache[cache_len++] = val;
}

/* Return the nth element of the sequence.
   Caller may pass any n ≥ 1; the function generates terms on demand. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for non‑positive indices */

    while (cache_len < (size_t)n) {
        seq_t candidate = last_tested + 1;
        if (candidate <= 0) candidate = 1;   /* safeguard against wrap‑around */

        if (qualifies(candidate)) {
            append_cache(candidate);
        }
        last_tested = candidate;
    }
    return cache[n - 1];
}
