/*
 * A044149  Numbers k such that string 1,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Sequence: numbers k such that the base-7 representation of k contains
   the substring 1,4 but the base-7 representation of k-1 does not. */

static bool has_14(seq_t x)
{
    /* Base‑7 digit scan from least significant to most significant.
       It returns true if a digit '1' is immediately followed by a digit '4'
       when walking from low to high positions, which is equivalent to
       finding the substring 1,4 in the normal left‑to‑right order. */
    if (x == 0) return false;
    seq_t prev = x % 7;
    x /= 7;
    while (x != 0) {
        seq_t curr = x % 7;
        if (curr == 1 && prev == 4) return true;
        prev = curr;
        x /= 7;
    }
    return false;
}

/* -------------------------------------------------------------
   Cached values: a static array that stores all terms computed
   so far.  The function is memoised; it can be called with
   indices in any order.
   ------------------------------------------------------------- */
static seq_t *cache        = NULL;
static size_t cap          = 0;   /* current capacity of cache */
static size_t len          = 0;   /* number of cached terms */
static seq_t last_checked  = 0;   /* last number examined */

/* -------------------------------------------------------------
   Return the nth term of A044149 (offset 1).
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* defensive guard – OEIS offset is 1 */

    while (len < n) {
        seq_t k = last_checked + 1;
        last_checked = k;

        if (has_14(k) && !has_14(k - 1)) {
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
#ifdef __cplusplus
                /* Allocate a larger array and copy the old elements with
                   proper copy‑semantics (handles arbitrary‑precision types). */
                seq_t *new_cache = new seq_t[new_cap];
                if (cache) {
                    for (size_t i = 0; i < cap; ++i)
                        new_cache[i] = cache[i];
                    delete[] cache;
                }
                cache = new_cache;
#else
                seq_t *new_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);   /* allocation failure */
                cache = new_cache;
#endif
                cap = new_cap;
            }
            cache[len++] = k;
        }
    }
    return cache[n - 1];
}
