/*
 * A184529  Upper s-Wythoff sequence, where s=5n-3. Complement of A184528.
 * https://oeis.org/A184529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the upper s‑Wythoff sequence with s = 5n–3.  The sequence
   is constructed by repeatedly choosing the smallest unused integer L_k
   as the lower element of the k‑th pair, adding s_k = 5k–3 to obtain the
   corresponding upper element U_k = L_k + s_k, and marking both numbers
   as used.  The function returns the upper element U_n. */
#include <stdlib.h>
#include <stdbool.h>

/* Ensure that 'arr' has at least 'needed' elements.  Use realloc for the
   dynamic array; the array holds seq_t values. */
static void
ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap > 0 ? *cap << 1 : 8);
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = newcap;
}

/* Ensure that the boolean table has indices up to 'needed'.
   The table tracks whether a positive integer has already been used. */
static void
ensure_used_cap(size_t *cap, bool **used, idx_t needed)
{
    if ((idx_t)*cap > needed)
        return;
    size_t newcap = (*cap > 0 ? *cap << 1 : 64);
    while ((idx_t)newcap <= needed)
        newcap <<= 1;
    bool *tmp = (bool *)realloc(*used, newcap * sizeof(bool));
    if (!tmp)
        abort();
    /* Initialise new entries to false. */
    if (*used == NULL) {
        for (size_t i = 0; i < newcap; ++i)
            tmp[i] = false;
    } else {
        for (size_t i = *cap; i < newcap; ++i)
            tmp[i] = false;
    }
    *used = tmp;
    *cap = newcap;
}

/* Static state for memoisation. */
static seq_t *upper_cache = NULL;   /* a(k) values, 0‑based */
static size_t upper_cap   = 0;
static size_t upper_cnt   = 0;      /* number of upper terms cached */

static bool *used_table = NULL;     /* tracks used integers */
static size_t used_cap  = 0;
static idx_t next_candidate = 1;    /* next integer to test for “unused” */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1 */

    /* Extend cache up to index n. */
    while (upper_cnt < (size_t)n) {
        /* Find the next unused integer (lower value). */
        while (true) {
            ensure_used_cap(&used_cap, &used_table, next_candidate);
            if (!used_table[(size_t)next_candidate])
                break;
            ++next_candidate;
        }
        seq_t lower = (seq_t)next_candidate;
        used_table[(size_t)next_candidate] = true;  /* mark lower used */

        /* Compute s = 5k – 3 for current k (1‑based). */
        idx_t k = (idx_t)upper_cnt + 1ULL;
        idx_t s = 5 * k - 3;

        /* Compute upper value and mark it used. */
        seq_t upper = lower + (seq_t)s;
        idx_t upper_index = (idx_t)upper;
        ensure_used_cap(&used_cap, &used_table, upper_index);
        used_table[(size_t)upper_index] = true;

        /* Store the computed upper in the cache. */
        ensure_cap(&upper_cache, &upper_cap, upper_cnt + 1);
        upper_cache[upper_cnt] = upper;

        /* Advance state for next iteration. */
        ++next_candidate;
        ++upper_cnt;
    }

    return upper_cache[(size_t)n - 1];
}
