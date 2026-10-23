/*
 * A193845  Mirror of the triangle A193844.
 * https://oeis.org/A193845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flat enumeration of the triangle T(n,k) defined by *
 *            T(0,0) = 1, * T(n,k) = 0 if k<0 or k>n, * T(n,k) = 3*T(n-1,k) +
 *            T(n-1,k-1) * - 2*T(n-2,k) - T(n-2,k-1), for n ≥ 2. * * The array
 *            `a` contains the first term of each row followed by the *
 *            remaining terms of the row, with the rows rev...
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 257, u64 n <= 915,
 *            u128 n <= 3509, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A193845_H_
#define _A193845_H_

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t, etc. */

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Cache and row buffers (static so that calls are order‑independent)*/
/* ------------------------------------------------------------------ */

#ifdef __cplusplus
static std::vector<seq_t> cache;          /* flattened triangle values      */
static std::vector<seq_t> row_prev;       /* T(n-2,*)                       */
static std::vector<seq_t> row_cur;        /* T(n-1,*)                       */
static idx_t last_row = (idx_t)-1;        /* last row that has been built   */
#else
/* In C the memory is managed manually with malloc/resize. */
static seq_t *cache        = NULL;  /* flattened triangle values */
static size_t cache_cap    = 0;      /* current capacity      */
static size_t cache_size   = 0;      /* number of values cached   */
static seq_t *row_prev     = NULL;
static size_t prev_len     = 0;
static seq_t *row_cur      = NULL;
static size_t cur_len      = 0;
static idx_t last_row      = (idx_t)-1;
#endif

/* ------------------------------------------------------------------ */
/*  Helper: Ensure the cache has room for at least `needed` elements  */
/* ------------------------------------------------------------------ */
#ifndef __cplusplus
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* ------------------------------------------------------------------ */
/*  Compute the next row of T(n,k) and append it to the cache.       */
/* ------------------------------------------------------------------ */
static void compute_next_row(void)
{
#ifdef __cplusplus
    size_t len_next = row_cur.size() + 1;
    std::vector<seq_t> row_next;
    row_next.resize(len_next);

    for (size_t k = 0; k < len_next; ++k) {
        seq_t t_n1_k   = (k < row_cur.size())   ? row_cur[k]   : (seq_t)0;
        seq_t t_n1_k1  = (k > 0 && k-1 < row_cur.size())
                           ? row_cur[k-1] : (seq_t)0;
        seq_t t_n2_k   = (k < row_prev.size())  ? row_prev[k]  : (seq_t)0;
        seq_t t_n2_k1  = (k > 0 && k-1 < row_prev.size())
                           ? row_prev[k-1] : (seq_t)0;

        row_next[k] = 3 * t_n1_k + t_n1_k1 - 2 * t_n2_k - t_n2_k1;
    }

    /* Append the new row to the cache */
    cache.insert(cache.end(), row_next.begin(), row_next.end());

    /* Shift row buffers */
    row_prev.swap(row_cur);      /* row_prev <- old row_cur */
    row_cur = std::move(row_next);   /* row_cur  <- new row      */
    ++last_row;
#else
    size_t len_next = cur_len + 1;
    seq_t *row_next = (seq_t*)malloc(len_next * sizeof(seq_t));
    if (!row_next)
        abort();                 /* out of memory */

    for (size_t k = 0; k < len_next; ++k) {
        seq_t t_n1_k   = (k < cur_len) ? row_cur[k] : (seq_t)0;
        seq_t t_n1_k1  = (k > 0 && k-1 < cur_len) ? row_cur[k-1] : (seq_t)0;
        seq_t t_n2_k   = (k < prev_len) ? row_prev[k] : (seq_t)0;
        seq_t t_n2_k1  = (k > 0 && k-1 < prev_len) ? row_prev[k-1] : (seq_t)0;

        row_next[k] = 3 * t_n1_k + t_n1_k1 - 2 * t_n2_k - t_n2_k1;
    }

    /* Append to cache */
    ensure_cache_capacity(cache_size + len_next);
    for (size_t i = 0; i < len_next; ++i)
        cache[cache_size + i] = row_next[i];
    cache_size += len_next;

    /* Advance row buffers */
    free(row_prev);
    row_prev = row_cur;
    prev_len = cur_len;

    row_cur = row_next;
    cur_len = len_next;
    ++last_row;
#endif
}

/* ------------------------------------------------------------------ */
/*  Public function returning the nth term of A193845                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (last_row == (idx_t)-1) {
#ifdef __cplusplus
        cache.clear();
        cache.shrink_to_fit();
        cache.reserve(64);
#else
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();             /* out of memory */
        cache_size = 0;
#endif
        last_row = 0;
#ifdef __cplusplus
        /* row 0: T(0,0) = 1 */
        row_cur.clear();
        row_cur.push_back((seq_t)1);
        row_prev.clear();       /* empty row for n-1 */
#else
        /* row 0: T(0,0) = 1 */
        row_cur = (seq_t*)malloc(sizeof(seq_t));
        if (!row_cur)
            abort();             /* out of memory */
        row_cur[0] = (seq_t)1;
        cur_len = 1;

        row_prev = NULL;
        prev_len = 0;
#endif

        /* store a(0) */
#ifdef __cplusplus
        cache.push_back((seq_t)1);
#else
        cache[cache_size++] = (seq_t)1;
#endif
    }

    /* Generate rows until we have at least n+1 terms */
    while ( /* in C++ we use size() */ 
#ifdef __cplusplus
          cache.size() <= (size_t)n
#else
          cache_size <= (size_t)n
#endif
          )
        compute_next_row();

#ifdef __cplusplus
    return cache[(size_t)n];
#else
    return cache[n];
#endif
}

#endif /* _A193845_H_ */
