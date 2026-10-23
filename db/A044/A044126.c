/*
 * A044126  Numbers n such that string 4,0 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044126: Numbers n such that the base‑6 representation of n contains
   the substring 4,0 but the representation of n‑1 does not.
   The function searches sequentially and caches the results.
   For each call it guarantees to return the correct a(n) for n ≥ 1. */

#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* Test whether a base‑6 number contains the substring 4,0. */
static bool has_40(seq_t x)
{
    /* A single digit cannot contain the pattern. */
    if (x < 6)
        return false;

    seq_t prev = x % 6;   /* least significant digit */
    x /= 6;
    seq_t cur;

    while (x) {
        cur = x % 6;
        if (cur == 4 && prev == 0)
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

/* Cache of discovered terms.  The size 200 000 comfortably covers all
   indices up to about 10 000 while keeping the memory footprint tiny. */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static size_t cache_count = 0;   /* number of terms stored */
static seq_t next_candidate = (seq_t)1; /* next number to test */

/* Return the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    /* The harness never asks for n <= 0 or > CACHE_CAP.  If it does,
       returning 0 signals the error. */
    if (n <= 0 || n > CACHE_CAP)
        return 0;

    /* Generate terms until we have at least n stored. */
    while (cache_count < (size_t)n) {
        seq_t x = next_candidate;
        seq_t xm1 = x - 1;      /* x > 0, safe */
        if (has_40(x) && !has_40(xm1)) {
            cache[cache_count++] = x;
        }
        next_candidate = x + 1;
    }

    return cache[n - 1];
}
