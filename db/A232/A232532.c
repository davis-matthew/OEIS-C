/*
 * A232532  Positive numbers not of the form x^2 - 3*y^2; complement of
 *          A084916.
 * https://oeis.org/A232532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A232532 is the set of positive integers not of the form x^2 –
 *            3*y^2. This implementation simply enumerates the natural
 *            numbers, tests each one for representability, and caches the
 *            non‑representable results. The cache is expanded lazily so that
 *            repeated calls with non‑monotonic indices are also ha...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/* Necessary headers                                           */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------- */
/* Integer square root for 64‑bit values (floor(sqrt(n))) */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t bit = 1ULL << 62;            /* 2^62 is the largest 4^k ≤ 2^64-1 */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else
            x >>= 1;
        bit >>= 2;
    }
    return x;
}

/* --------------------------------------------------------------- */
/* Cache structures                                            */
static seq_t *seq_cache   = NULL;   /* array of non‑representables   */
static size_t seq_len     = 0;      /* number of stored terms        */
static size_t seq_cap     = 0;      /* allocated capacity            */
static idx_t  next_val    = 1;      /* next candidate integer        */

/* --------------------------------------------------------------- */
/* Test whether n can be written as x^2 – 3*y^2.  Return true if
   such integers exist.                                           */
static bool is_representable(idx_t n)
{
    idx_t y_max = floor_isqrt_u64(n / 3);   /* y ≤ floor(sqrt(n/3)) */
    for (idx_t y = 0; y <= y_max; ++y) {
        idx_t t = n + 3ULL * y * y;          /* n + 3*y^2             */
        idx_t s = floor_isqrt_u64(t);
        if (s * s == t) return true;        /* perfect square found  */
    }
    return false;                            /* no representation     */
}

/* --------------------------------------------------------------- */
/* Public sequence function.  Builds the cache until it
   contains at least n terms.                                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    while (seq_len < (size_t)n) {
        if (!is_representable(next_val)) {
            if (seq_len == seq_cap) {           /* grow cache */
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *new_seq = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                seq_cache = new_seq;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_val;   /* store term */
        }
        ++next_val;   /* advance to next integer */
    }
    return seq_cache[n-1];  /* 1‑based indexing as required */
}
