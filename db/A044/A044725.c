/*
 * A044725  Numbers n such that string 1,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer such that its decimal representation
   contains the substring "12" but the next integer n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>

/* return true iff decimal representation of x contains the pair 1 followed by 2 */
static bool has12(seq_t x)
{
    while (x > (seq_t)0) {
        seq_t d      = x % (seq_t)10;          /* least significant digit   */
        seq_t next   = (x / (seq_t)10) % (seq_t)10;  /* next more significant     */
        if (next == (seq_t)1 && d == (seq_t)2)
            return true;
        x = x / (seq_t)10;
    }
    return false;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static bool   initialized = false;
    static seq_t next_candidate = 0;

    if (!initialized) {
        initialized        = true;
        cache_cap          = 64;
        cache              = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_candidate     = (seq_t)1;      /* start checking from 1       */
    }

    /* Extend cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        if (has12(next_candidate) && !has12(next_candidate + (seq_t)1)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
