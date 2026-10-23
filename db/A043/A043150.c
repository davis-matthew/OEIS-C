/*
 * A043150  Numbers k such that 4 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 4 and 6 occur juxtaposed in the base‑7 representation
 * of k but not of k‑1. */
#include <stdbool.h>
#include <stdlib.h>

/* Return true if the base‑7 representation of x contains the pair
 * 4 and 6 next to each other in either order. */
static bool has_46(seq_t x)
{
    if (x == 0)
        return false;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 7);      /* least significant digit */
        if (prev == 6 && d == 4)   /* ...4,6... */
            return true;
        if (prev == 4 && d == 6)   /* ...6,4... */
            return true;
        prev = d;
        x = x / 7;
    }
    return false;
}

/* Cache of already found sequence terms. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;   /* number of terms currently stored   */
static size_t seq_cap   = 0;   /* allocated capacity                  */
static seq_t last_checked = 0; /* largest integer examined so far     */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t cand = last_checked + 1;
        if (has_46(cand) && !has_46(cand - 1)) {
            /* Ensure there is space. */
            if (seq_len >= seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                /* explicit cast to satisfy C++ */
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* unrecoverable memory failure */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = cand;
        }
        last_checked = cand;
    }
    return seq_cache[n - 1];
}
