/*
 * A068865  a(1) = 1; a(n) is the smallest triangular number not already in
 *          the sequence which differs from a(n-1) at every digit.
 * https://oeis.org/A068865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068865: a(n) = the smallest unused triangular number that differs from a(n‑1) in every decimal digit.
   a(1) = 1.  For n > 1, find the minimal unused T(k) = k(k+1)/2 such that every digit of T(k)
   differs from the corresponding digit of a(n‑1) (missing digits are treated as 0). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Auxiliary: test if all decimal digits of a and b differ.
   Missing higher‑order digits are treated as 0. */
static bool diff_all_digits(seq_t a, seq_t b)
{
    while (a > 0 || b > 0) {
        unsigned da = (a > 0) ? (unsigned)(a % 10) : 0;
        unsigned db = (b > 0) ? (unsigned)(b % 10) : 0;
        if (da == db) return false;
        a = (a > 0) ? a / 10 : 0;
        b = (b > 0) ? b / 10 : 0;
    }
    return true;
}

/* Ensure the boolean array "used" can index at least k. */
static void ensure_used_idx(idx_t k, bool **used, size_t *used_size)
{
    size_t idx = (size_t)k;
    if (idx >= *used_size) {
        size_t new_size = *used_size ? *used_size * 2 : 16;
        while (new_size <= idx) new_size <<= 1;
        bool *new_used = (bool *)realloc(*used, new_size * sizeof(bool));
        if (!new_used) abort();
        memset(new_used + *used_size, 0, (new_size - *used_size) * sizeof(bool));
        *used = new_used;
        *used_size = new_size;
    }
}

/* =======================================
   Main sequence generator
   ======================================= */
static seq_t *seq = NULL;          /* 1‑based array of already computed terms   */
static size_t seq_size = 0;        /* allocated size of seq  (>= n+1)           */
static idx_t  seq_used = 0;        /* highest index that has a value in seq    */

static bool *used = NULL;          /* used[k]  == true  ⇔  T(k) already used    */
static size_t used_size = 0;        /* allocated size of used (>= max index seen) */

static bool initialized = false;   /* one‑time initialization flag */

static void init_once(void)
{
    if (initialized) return;
    initialized = true;

    /* Initial space for seq[1] */
    seq_size = 16;
    seq = mem_new(seq_t, seq_size);
    if (!seq) abort();
    mem_set(seq, 0, seq_size * sizeof(seq_t));

    seq[1] = (seq_t)1;  /* a(1) = 1 */
    seq_used = 1;

    /* Space for used indices */
    used_size = 16;
    used = (bool *)calloc(used_size, sizeof(bool));
    if (!used) abort();
    used[1] = true;   /* T(1)=1 is used */
}

/* Computes a(n) for any n >= 1, calls may be out of order. */
seq_t a(idx_t n)
{
    init_once();
    if (n == 0) return (seq_t)0;   /* undefined, but defensive */

    /* Grow seq[] if needed */
    if ((size_t)n >= seq_size) {
        size_t new_size = seq_size ? seq_size * 2 : 16;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *new_seq = mem_resize(seq_t, seq, new_size);
        if (!new_seq) abort();
        mem_set(new_seq + seq_size, 0, (new_size - seq_size) * sizeof(seq_t));
        seq = new_seq;
        seq_size = new_size;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = seq_used + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        idx_t k = 1;          /* start from the smallest unused index */

        /* Find the smallest unused triangular number differing from prev. */
        for (;;) {
            /* Ensure used[] covers current k */
            ensure_used_idx(k, &used, &used_size);

            if (used[k]) {          /* already used, skip */
                ++k;
                continue;
            }

            seq_t val = (seq_t)k * ((seq_t)k + 1) / 2;
            if (diff_all_digits(prev, val)) {
                seq[i] = val;        /* store the term */
                used[k] = true;      /* mark this triangular number as used */
                break;
            }
            ++k;   /* try next triangular number */
        }
    }

    seq_used = n;
    return seq[(size_t)n];
}
