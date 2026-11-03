/*
 * A044147  Numbers k such that string 1,2 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044147: numbers k such that the base‑7 representation of k
   contains the substring "12" but the representation of k‑1 does not.
   The function generates terms by a simple exhaustive scan. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>   /* for size_t    */
#include "../utils/mem.h"

/* Return true if base‑7 representation of x contains digits 1 followed by 2. */
static bool has12_seq_t(seq_t x)
{
    const seq_t seven = (seq_t)7;
    seq_t prev = (seq_t)-1;                     /* sentinel for first digit   */
    if (x == (seq_t)0) return false;           /* 0 has representation "0" */
    while (x > (seq_t)0) {
        seq_t d = x % seven;
        if (prev == (seq_t)2 && d == (seq_t)1) return true;
        prev = d;
        x /= seven;
    }
    return false;
}

/* Return the n‑th term of A044147 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (elements)   */
    static size_t cache_len = 0;   /* number of terms stored          */
    static idx_t  next_k     = 1;   /* next integer to examine         */
    static bool   prev_has12 = false; /* has12(next_k-1)                 */

    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cur = (seq_t)next_k;
        bool cur_has12 = has12_seq_t(cur);

        if (cur_has12 && !prev_has12) {
            /* Grow cache if necessary. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();                  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        prev_has12 = cur_has12;
        next_k++;   /* proceed to next integer */
    }

    return cache[n - 1];
}
