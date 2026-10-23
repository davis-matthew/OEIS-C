/*
 * A165165  Numbers without repeated digits that yield only integers when the
 *          sum of the leftmost k digits is divided by k for all k = 1, 2, ...
 * https://oeis.org/A165165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 194, u32 n <= 673, u64 n <= 673, u128 n <= 673,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165165: numbers without repeated digits that satisfy the prefix‑sum divisibility condition.
   Computed by a depth‑first search and sorted. */

#include <stdlib.h>
#include <stdbool.h>

/* Append a value to a dynamic array, growing as needed. */
static void append_term(seq_t **arr, size_t *cap, size_t *cnt, seq_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failed – ignore this term */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*cnt] = val;
    (*cnt)++;
}

/* Depth‑first generation of admissible numbers. */
static void dfs(seq_t cur, int cur_len, int cur_sum, uint16_t used,
                seq_t **arr, size_t *cap, size_t *cnt)
{
    for (int d = 0; d <= 9; ++d) {
        if (used & (1u << d)) continue;
        if (cur_len == 0 && d == 0) continue;   /* no leading zero */
        int new_len  = cur_len + 1;
        int new_sum  = cur_sum + d;
        if (new_sum % new_len != 0) continue;    /* prefix rule fails */
        seq_t new_val = cur * 10 + (seq_t)d;
        append_term(arr, cap, cnt, new_val);
        dfs(new_val, new_len, new_sum, used | (1u << d), arr, cap, cnt);
    }
}

/* Comparison function for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t va = *(const seq_t *)a;
    const seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Public API: a(n) returns the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *terms   = NULL;   /* array of found terms */
    static size_t term_sz = 0;      /* number of terms */
    static size_t term_cap = 0;     /* allocated capacity */

    if (!initialized) {
        /* generate all admissible numbers */
        dfs(0, 0, 0, 0, &terms, &term_cap, &term_sz);
        /* sort ascending to match OEIS order */
        qsort(terms, term_sz, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    if (n == 0 || n > term_sz) return (seq_t)0;  /* out of range */
    return terms[n - 1];
}
