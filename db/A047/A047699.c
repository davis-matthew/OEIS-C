/*
 * A047699  a(0) = 0; for n >= 0, a(n+1) = a(n) + x where x is the smallest
 *          nonnegative number that is not equal to a(i) +- a(k) for any 0 <=
 *          i <= n, 0 <= k <= n.
 * https://oeis.org/A047699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 202, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A047699:  Sparse maximal expulsion set under addition.
 *
 *  a(0) = 0;
 *  for n >= 0,
 *    a(n+1) = a(n) + x,  where
 *    x = smallest non‑negative integer not equal to
 *        a(i) ± a(k) for any 0 ≤ i ≤ n, 0 ≤ k ≤ n.
 *
 *  The implementation keeps the sequence in a static vector and
 *  maintains a set of all excluded values (a(i)+a(k) and |a(i)-a(k)|).
 *  For the C build it uses a grow‑able bit‑set; for the C++ (big‑int)
 *  build it uses std::set, which does not require a default
 *  constructor for the value type.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <set>
#endif

/* ------------------------------------------------------------------ */
/*  Global state (shared by all calls)                                 */
/* ------------------------------------------------------------------ */

/* Sequence storage */
static seq_t *seq  = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Exclusion set ----------------------------------------------- */
#ifdef __cplusplus
/* C++: use std::set (requires only copy‑constructible & orderable) */
static std::set<seq_t> excluded;

/* Insert value into exclusion set */
static inline void present_set(const seq_t &v)
{
    excluded.insert(v);
}

/* Test if value is present */
static inline bool present_test(const seq_t &v)
{
    return excluded.find(v) != excluded.end();
}
#else
/* C: use bit‑set of uint64_t words */
static uint64_t *present   = NULL;     /* bit array */
static size_t   present_cap = 0;       /* number of bits currently usable */

/* Ensure bit‑set can accommodate index 'v' (inclusive) */
static void ensure_present_size(idx_t v)
{
    if ((size_t)v >= present_cap) {
        size_t required_words = ((size_t)v + 63) / 64;
        size_t new_cap = present_cap ? present_cap / 64 : 1;
        while (new_cap < required_words) new_cap <<= 1;

        uint64_t *new_buf = realloc(present, new_cap * sizeof(uint64_t));
        if (!new_buf) abort();               /* allocation failure */

        if (new_buf != present) {
            present = new_buf;
        }
        /* zero the newly allocated words */
        memset(present + present_cap / 64, 0,
               (new_cap - present_cap / 64) * sizeof(uint64_t));
        present_cap = new_cap * 64;
    }
}

/* Insert value into exclusion set */
static inline void present_set(const seq_t v)
{
    idx_t idx = (idx_t)v;          /* assumes v fits in idx_t */
    ensure_present_size(idx);
    size_t w = idx >> 6;
    size_t b = idx & 63;
    present[w] |= ((uint64_t)1u << b);
}

/* Test if value is present */
static inline bool present_test(const seq_t v)
{
    idx_t idx = (idx_t)v;
    if ((size_t)idx >= present_cap) return false;
    size_t w = idx >> 6;
    size_t b = idx & 63;
    return ((present[w] >> b) & 1u) != 0;
}
#endif

/* ------------------------------------------------------------------ */
/*  Helpers for sequence storage                                      */
/* ------------------------------------------------------------------ */
static void ensure_seq_cap(size_t need)
{
    if (need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_buf = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_buf) abort();        /* allocation failure */
        seq = new_buf;
        seq_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --- Initialise if first call --- */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)0;
        seq_len = 1;

        /* Insert 0 into exclusion set (0 = a(0) ± a(0)) */
        present_set((seq_t)0);
    }

    /* --- Build sequence up to index n --- */
    while (seq_len <= n) {
        /* Find smallest non‑negative integer not in the exclusion set */
        idx_t x = 0;
        while (present_test((seq_t)x)) ++x;

        /* New term = previous + x */
        seq_t new_val = seq[seq_len - 1] + (seq_t)x;

        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = new_val;
        seq_len++;

        /* Mark a(i) ± new_val for all previous i */
        seq_t v = new_val;
        for (size_t i = 0; i < seq_len - 1; ++i) {
            seq_t ai = seq[i];

            /* Sum v + ai */
            present_set(v + ai);

            /* Absolute difference |v - ai| */
            if (v >= ai) {
                present_set(v - ai);
            } else {
                present_set(ai - v);
            }
        }

        /* Also exclude 2*v = v + v (self‑sum) */
        present_set(v + v);
    }

    return seq[n];
}
