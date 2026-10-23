/*
 * A044021  Numbers k such that 3 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044021: Numbers k such that 3 and 5 occur juxtaposed in the
    base‑10 representation of k but not of k+1. */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_CACHE 12000   /* sufficient for n up to ~10000 */

/* Test whether x contains the digit pair 35 or 53. */
static bool has35or53(seq_t x)
{
    while (x >= 10) {
        seq_t pair = x % 100;
        if (pair == 35 || pair == 53)
            return true;
        x /= 10;
    }
    return false;
}

/* Static cache of found terms. */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t candidate = 35;  /* current integer to test */

/* Ensure that the cache holds at least n terms. */
static void ensure_terms(idx_t n)
{
    /* Safety: if n > MAX_CACHE, simply stop; the function will abort
       later when an out‑of‑bounds index is requested. */
    if (cache_len >= (size_t)n)
        return;

    while (cache_len < (size_t)n && cache_len < MAX_CACHE) {
        while (true) {
            if (has35or53(candidate) && !has35or53(candidate + 1)) {
                cache[cache_len++] = candidate;
                candidate++;            /* move to next number */
                break;
            }
            candidate++;                /* skip numbers that don't match */
        }
    }
}

/* Return the n-th term of A044021.  n is 1‑based; n==0 is invalid. */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();                       /* invalid index – abort */
    if (n > MAX_CACHE)
        abort();                       /* out‑of‑bounds cache size */
    ensure_terms(n);
    if (cache_len < (size_t)n)
        abort();                       /* safety but should never happen */
    return cache[(size_t)n - 1];
}
