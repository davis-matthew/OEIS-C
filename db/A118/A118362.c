/*
 * A118362  a(1)=1; for n>1, a(n) = "n AND a(n-1)" if that number is positive
 *          and not already in the sequence, otherwise a(n) = "n OR a(n-1)".
 * https://oeis.org/A118362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n) = n AND a(n-1) if that value is positive
 *            and new, otherwise a(n) = n OR a(n-1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ---------- dynamic array helpers ---------- */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    void *new_mem = realloc(*ptr, new_cap * elem_size);
    if (!new_mem) abort();          /* allocation failed */
    *ptr = new_mem;
    *cap = new_cap;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_vals = NULL;    /* seq_vals[0] corresponds to a(1) */
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* ---------- list of values that have appeared ---------- */
static seq_t *seen_vals = NULL;
static size_t seen_len = 0;
static size_t seen_cap = 0;

/* ---------- auxiliary functions ---------- */
static bool seen_contains(seq_t v)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (seen_vals[i] == v) return true;
    return false;
}

static void seen_add(seq_t v)
{
    if (seen_contains(v)) return;
    ensure_capacity(seen_len + 1, &seen_cap, (void **)&seen_vals, sizeof(seq_t));
    seen_vals[seen_len++] = v;
}

static void ensure_initialized(void)
{
    if (seq_vals) return;      /* already initialized */

    /* start the sequence with a(1) = 1 */
    ensure_capacity(1, &seq_cap, (void **)&seq_vals, sizeof(seq_t));
    seq_vals[0] = (seq_t)1;
    seq_len = 1;

    /* start the list of seen values with 1 */
    ensure_capacity(1, &seen_cap, (void **)&seen_vals, sizeof(seq_t));
    seen_vals[0] = (seq_t)1;
    seen_len = 1;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    ensure_initialized();

    if ((size_t)n <= seq_len) return seq_vals[(size_t)n - 1];

    /* compute terms up to n */
    for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {
        seq_t prev = seq_vals[(size_t)i - 2];
        seq_t cand = ((seq_t)i) & prev;       /* n AND a(n-1) */

        seq_t val;
        if (cand != (seq_t)0 && !seen_contains(cand))
            val = cand;
        else
            val = ((seq_t)i) | prev;          /* n OR a(n-1) */

        ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq_vals, sizeof(seq_t));
        seq_vals[seq_len++] = val;

        if (val != (seq_t)0) seen_add(val);
    }

    return seq_vals[(size_t)n - 1];
}
