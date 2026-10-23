/*
 * A043760  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 6 mod 9.
 * https://oeis.org/A043760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number whose binary representation has number of
 *            runs congruent to 6 modulo 9
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches integers sequentially, keeping a cache of
   the qualified numbers.  It is O(n) in the value of a(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Allocate more space for the cache when needed. */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 64;
    if (needed > new_cap)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* memory allocation failed */
    *arr = tmp;
    *cap = new_cap;
}

/* Return the n-th term of A043760 (for n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached qualifying numbers   */
    static size_t len   = 0;      /* number of terms stored      */
    static size_t cap   = 0;      /* current capacity of cache   */
    static uint64_t last_checked = 0;  /* largest integer examined */

    if (n == 0)
        return 0;                 /* undefined, but harmless      */

    /* Grow the cache until it holds at least n terms. */
    if (len < (size_t)n) {
        ensure_capacity(n, &cache, &cap);

        uint64_t x;
        for (x = last_checked + 1; len < (size_t)n; ++x) {
            uint64_t t = x;
            int runs   = 0;
            int prev   = 0;
            bool first = true;

            /* Count runs in the binary representation of t. */
            while (t) {
                int bit = t & 1;
                if (first) {
                    runs   = 1;
                    first  = false;
                } else if (bit != prev) {
                    ++runs;
                }
                prev = bit;
                t >>= 1;
            }

            if (runs % 9 == 6) {
                cache[len++] = (seq_t)x;   /* store the qualifying n‑tuple */
            }
        }
        last_checked = x - 1;              /* last examined integer  */
    }

    return cache[(size_t)n - 1];
}
