/*
 * A044276  Numbers n such that string 2,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: numbers n whose base‑9 representation contains "27" but n‑1 does not.
 * This implementation searches forward while caching discovered terms.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x (in base 9) contains the substring "27" */
static bool has_27(seq_t x)
{
    if (x == 0) return false;

    seq_t prev = x % 9;
    x /= 9;
    if (x == 0) return false;

    seq_t curr = x % 9;
    if (prev == 7 && curr == 2) return true;
    prev = curr;
    x /= 9;

    while (x > 0) {
        curr = x % 9;
        if (prev == 7 && curr == 2) return true;
        prev = curr;
        x /= 9;
    }
    return false;
}

/* Static cache for already found terms */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;   /* number of stored terms  */
static size_t cache_cap  = 0;   /* allocated capacity      */
static seq_t  scan_next  = 1;   /* next integer to test    */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */

    size_t target = (size_t)n;
    while (cache_sz < target) {
        seq_t val = scan_next;          /* current candidate      */
        scan_next += 1;                 /* advance scan pointer   */

        if (has_27(val) && !has_27(val - 1)) {
            /* grow cache if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {            /* allocation failure */
                    return 0;
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = val;    /* store found term */
        }
    }

    return cache[n - 1];
}
