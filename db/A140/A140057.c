/*
 * A140057  Numbers such that each decimal digit d (for all d in the range 1
 *          <= d <= 9) occurs only in runs of length exactly d.
 * https://oeis.org/A140057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 257, u64 n <= 8032,
 *            u128 n <= 8032, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140057 – numbers in which every decimal digit d (1 ≤ d ≤ 9)
   appears only in blocks of exactly d consecutive copies.
   The implementation generates all such numbers in increasing
   numeric order by recursively building blocks of length d
   and sorting the results of each decimal length. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/pow10.h"

/* Global storage for generated terms */
static seq_t *seq = NULL;          /* array of terms */
static size_t seq_cap = 0;         /* capacity of seq array */
static size_t seq_len = 0;         /* number of terms stored */

/* Temporary buffer for terms of a specific length */
static seq_t *temp = NULL;
static size_t temp_cap = 0;
static size_t temp_len = 0;

/* Pre‑computed values of d repeated d times, for d = 1 … 9 */
static seq_t repeat_val[10];
static int repeat_inited = 0;

/* ------------------------------------------------------------------ */
/*  Utility functions for dynamic array growth                       */
/* ------------------------------------------------------------------ */

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = new_cap;
    }
}

static void ensure_temp_cap(size_t needed)
{
    if (temp_cap < needed) {
        size_t new_cap = temp_cap == 0 ? 16 : temp_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(temp, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        temp = tmp;
        temp_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Current value push functions                                       */
/* ------------------------------------------------------------------ */

static void temp_push(seq_t v)
{
    ensure_temp_cap(temp_len + 1);
    temp[temp_len++] = v;
}

static void seq_push(seq_t v)
{
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = v;
}

/* ------------------------------------------------------------------ */
/*  Pre‑compute repeat values                                          */
/* ------------------------------------------------------------------ */

static void init_repeat(void)
{
    if (!repeat_inited) {
        for (int d = 1; d <= 9; ++d) {
            seq_t v = 0;
            for (int i = 0; i < d; ++i) v = v * 10 + d;
            repeat_val[d] = v;
        }
        repeat_inited = 1;
    }
}

/* ------------------------------------------------------------------ */
/*  Recursive construction of numbers of a given remaining length    */
/* ------------------------------------------------------------------ */

static void dfs(idx_t rem, int prev, seq_t cur)
{
    if (rem == 0) {
        temp_push(cur);
        return;
    }
    for (int d = 1; d <= 9 && (idx_t)d <= rem; ++d) {
        if (d == prev) continue;             /* no consecutive blocks of same digit */
        seq_t next = cur * pow10(d) + repeat_val[d];
        dfs(rem - d, d, next);
    }
}

/* ------------------------------------------------------------------ */
/*  Comparison function for qsort (works for any unsigned seq_t type) */
/* ------------------------------------------------------------------ */

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Generate terms of a specific decimal length, sort, and store      */
/* ------------------------------------------------------------------ */

static void generate_len(idx_t len)
{
    temp_len = 0;
    dfs(len, -1, (seq_t)0);

    if (temp_len > 1)
        qsort(temp, temp_len, sizeof(seq_t), cmp_seq);

    for (size_t i = 0; i < temp_len; ++i)
        seq_push(temp[i]);
}

/* ------------------------------------------------------------------ */
/*  Public function required by the harness                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n=0 is undefined */

    init_repeat();

    static idx_t cur_len = 0;   /* largest decimal length generated */

    /* Generate terms until we have at least n terms */
    while (seq_len < n) {
        ++cur_len;
        generate_len(cur_len);
    }

    return seq[n - 1];
}
