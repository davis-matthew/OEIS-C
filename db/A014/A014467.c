/*
 * A014467  Triangular array formed from elements to right of middle of rows
 *          of the triangle of Eulerian numbers.
 * https://oeis.org/A014467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: compute Eulerian numbers A(n,k) via
   A(n,k) = (k+1)·A(n-1,k) + (n-k)·A(n-1,k-1),
   collect the elements strictly to the right of the middle of each
   Eulerian row (the “right half”), unfold row by row, and return the
   n‑th element (1‑based).  a(1)=1.  */
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
/* mul_add: for integral seq_t use 128‑bit intermediate, otherwise use
   seq_t operations (big‑integer class). */
template<class T>
static inline T mul_add(T a, unsigned int mul1, T b, unsigned int mul2) {
    if constexpr (std::is_integral<T>::value) {
        unsigned __int128 t = (unsigned __int128)a * mul1 + (unsigned __int128)b * mul2;
        return static_cast<T>(t);
    } else {
        return a * mul1 + b * mul2;
    }
}
#else
/* In C all seq_t are assumed to be builtin integer types. */
static inline seq_t mul_add(seq_t a, unsigned int mul1, seq_t b, unsigned int mul2) {
    unsigned __int128 t = (unsigned __int128)a * mul1 + (unsigned __int128)b * mul2;
    return (seq_t)t;
}
#endif

/* Grow cache to hold at least 'needed' elements. */
static inline void grow_cache(idx_t needed, seq_t **cache, size_t *cap) {
    if ((idx_t)*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap)*2 : 16;
    if ((idx_t)newcap < needed) newcap = needed;
    *cache = (seq_t*)realloc(*cache, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Compute the n-th term of A014467. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* outside offset, arbitrary */

    /* Static cache for memoisation across calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t *prev = NULL;     /* previous Eulerian row */
    static idx_t prev_len = 0;
    static idx_t next_row = 1;     /* rows generated so far (next to generate) */

    /* Grow cache until it contains the n-th value. */
    while (cache_len < n) {
        idx_t row = next_row;               /* length of the current Eulerian row */
        seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * row);

        /* Compute Eulerian numbers for this row. */
        curr[0] = (seq_t)1;
        for (idx_t k = 1; k < row; ++k) {
            seq_t a1 = (k < prev_len) ? prev[k] : (seq_t)0;       /* A(row-1,k)   */
            seq_t a0 = (k > 0)        ? prev[k-1] : (seq_t)0;     /* A(row-1,k-1) */
            curr[k] = mul_add(a1, (unsigned int)(k+1), a0, (unsigned int)(row - k));
        }

        /* Add the right‑hand half of this row to the cache. */
        idx_t start = (row % 2 == 0) ? row/2 : (row+1)/2;
        for (idx_t i = start; i < row; ++i) {
            grow_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = curr[i];
        }

        /* Prepare for the next iteration. */
        free(prev);
        prev = curr;
        prev_len = row;
        ++next_row;
    }

    return cache[n-1];
}
