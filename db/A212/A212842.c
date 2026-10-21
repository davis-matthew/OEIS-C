/*
 * A212842  Numbers expressible as sum of squares of two distinct positive
 *          triangular numbers exactly in two ways.
 * https://oeis.org/A212842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 130, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A212842
    Numbers expressible as sum of squares of two distinct positive triangular numbers
    exactly in two ways.  The function b(n) returns the nth term (1‑based).  The
    implementation builds all unordered pairs T(i)^2+T(j)^2 with i<j up to a
    triangular index M, counts occurrences, and keeps those values that occur
    exactly twice.  It grows M until at least n such values are found.  The code
    works for both C and C++ builds; for C++ it uses std::vector, for C it
    allocates with malloc/realloc.  All arithmetic is performed in seq_t, which
    may be an unsigned integer type or an arbitrary‑precision integer class.  */

#ifndef A212842_H
#define A212842_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Adapt for C++ big‑integer builds ---------- */
#ifdef __cplusplus
#include <vector>
#endif

/* ---------- Wrapper for allocating and freeing arrays of seq_t ---------- */
#ifdef __cplusplus
static inline seq_t* alloc_seq_array(size_t n) { return new seq_t[n]; }
static inline void free_seq_array(seq_t* p)          { delete[] p; }
#else
static inline seq_t* alloc_seq_array(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static inline void free_seq_array(seq_t* p)          { free(p); }
#endif

/* ---------- Comparator for qsort (used only in C builds) ---------- */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ---------- Dynamic array helper for C builds ---------- */
#ifndef __cplusplus
static void append_seq_t(seq_t **buf, size_t *len, size_t *cap, seq_t val)
{
    if (*cap == 0) {
        *cap = 256;
        *buf = (seq_t*)malloc(*cap * sizeof(seq_t));
    } else if (*len == *cap) {
        *cap <<= 1;
        *buf = (seq_t*)realloc(*buf, *cap * sizeof(seq_t));
    }
    (*buf)[(*len)++] = val;
}
#endif

/* ---------- Global state ---------- */
static idx_t current_M = 0;      /* largest triangular index processed */

#ifdef __cplusplus
static std::vector<seq_t> terms_vec; /* store values that occur exactly twice */
#else
static seq_t *terms = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;
#endif

/* ---------- Rebuild tables up to new_M ---------- */
static void rebuild_terms(idx_t new_M)
{
    /* number of unordered pairs (i<j) for i,j in [1,new_M] */
    size_t pair_cnt = (size_t)new_M * (new_M - 1) / 2;

    /* allocate array for all sums */
    seq_t *sums = alloc_seq_array(pair_cnt);
    size_t pos = 0;

    /* generate all sums T(i)^2 + T(j)^2 */
    for (idx_t i = 1; i <= new_M; ++i) {
        seq_t ti = (seq_t)i * ((seq_t)i + 1) / 2;
        seq_t ti2 = ti * ti;
        for (idx_t j = i + 1; j <= new_M; ++j) {
            seq_t tj = (seq_t)j * ((seq_t)j + 1) / 2;
            sums[pos++] = ti2 + tj * tj;
        }
    }

    /* sort the sums */
#ifdef __cplusplus
    std::sort(sums, sums + pair_cnt);
#else
    qsort(sums, pair_cnt, sizeof(seq_t), cmp_seq_t);
#endif

    /* clear previous terms collection */
#ifdef __cplusplus
    terms_vec.clear();
#else
    free_seq_array(terms);
    terms = NULL;
    term_len = 0;
    term_cap = 0;
#endif

    /* scan sorted list and keep values that appear exactly twice */
    size_t i = 0;
    while (i < pair_cnt) {
        seq_t val = sums[i];
        size_t cnt = 1;
        ++i;
        while (i < pair_cnt && sums[i] == val) {
            ++cnt;
            ++i;
        }
        if (cnt == 2) {
#ifdef __cplusplus
            terms_vec.push_back(val);
#else
            append_seq_t(&terms, &term_len, &term_cap, val);
#endif
        }
    }

    free_seq_array(sums);
    current_M = new_M;
}

/* ---------- Ensure at least n terms are available ---------- */
static void ensure_terms(idx_t n)
{
    /* obtain at least n terms */
    if (n <= (idx_t)(current_M == 0 ? 0 :
#if defined(__cplusplus)
                      terms_vec.size()
#else
                      term_len
#endif
                     )) return;

    /* start with a modest M if nothing built yet */
    idx_t M = (current_M == 0) ? 100 : current_M;

    while (1) {
#ifdef __cplusplus
        if (terms_vec.size() >= (size_t)n) break;
#else
        if (term_len >= (size_t)n) break;
#endif
        M += M / 2 + 50;          /* increase by roughly 50% + offset */
        rebuild_terms(M);
    }
}

/* ---------- Public interface required by the test harness ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined; return 0 for safety */
    ensure_terms(n);
#ifdef __cplusplus
    return terms_vec[(size_t)(n - 1)];
#else
    return terms[(size_t)(n - 1)];
#endif
}

#endif /* A212842_H */
