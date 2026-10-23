/*
 * A133325  Numbers such that 2*X^2-82 is a square.
 * https://oeis.org/A133325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  each subsequence (odd indices and even indices) satisfies
   a(n+2) = 6*a(n+1) – a(n).
   Odd subsequence seeds: a1 = 7, a3 = 29.
   Even subsequence seeds: a2 = 13, a4 = 71.  */

#include <stdlib.h>

/* Dynamic array helpers ----------------------------------------------------- */

static void ensure_cache(seq_t **cache, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap == 0) ? 4 : (*cap * 2);
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
    if (!tmp) return;                 /* allocation failure – unlikely */
    *cache = tmp;
    *cap   = newcap;
}

/* ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    static seq_t *odd_cache  = NULL;   /* a(1), a(3), a(5), …    */
    static seq_t *even_cache = NULL;   /* a(2), a(4), a(6), …    */
    static size_t odd_cap   = 0, even_cap   = 0;
    static size_t odd_size  = 0, even_size  = 0;

    if (n == 0) return 0;              /* undefined, but safe */

    /* ---------- odd indices ---------- */
    if (n & 1U) {
        size_t idx = (n + 1U) >> 1;     /* 1‑based position in odd subsequence */
        if (odd_size < idx) {
            ensure_cache(&odd_cache, &odd_cap, idx);
            if (odd_size == 0) {
                odd_cache[0] = (seq_t)7;          /* a1   */
                odd_cache[1] = (seq_t)29;         /* a3   */
                odd_size = 2;
            } else if (odd_size == 1) {
                odd_cache[1] = (seq_t)29;
                odd_size = 2;
            }
            /* Compute missing terms starting from current size */
            for (size_t i = odd_size; i < idx; ++i) {
                odd_cache[i] = 6 * odd_cache[i - 1] - odd_cache[i - 2];
            }
            odd_size = idx;
        }
        return odd_cache[idx - 1];
    }

    /* ---------- even indices ---------- */
    else {
        size_t idx = n >> 1;          /* 1‑based position in even subsequence */
        if (even_size < idx) {
            ensure_cache(&even_cache, &even_cap, idx);
            if (even_size == 0) {
                even_cache[0] = (seq_t)13;          /* a2   */
                even_cache[1] = (seq_t)71;         /* a4   */
                even_size = 2;
            } else if (even_size == 1) {
                even_cache[1] = (seq_t)71;
                even_size = 2;
            }
            for (size_t i = even_size; i < idx; ++i) {
                even_cache[i] = 6 * even_cache[i - 1] - even_cache[i - 2];
            }
            even_size = idx;
        }
        return even_cache[idx - 1];
    }
}
