/*
 * A076105  a(1)=1 (first row) and then the n-th row of this triangle contains
 *          the least set of n unused natural numbers whose sum is a multiple
 *          of the sum of the previous row.
 * https://oeis.org/A076105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076105: the n‑th term of the triangle formed by
 * the smallest unused natural numbers.  Each row contains 
 * the first (len‑1) smallest unused numbers and a last
 * number chosen so that the row sum is a multiple of the
 * sum of the previous row.  The sequence a(n) is the
 * concatenation of all rows.
 * See OEIS A076105 for details. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static idx_t used_cap = 0;          /* highest index allocated in used[] (inclusive) */
static uint8_t *used = NULL;        /* bitmap: used[i] == 1 iff i already used */

static size_t vals_cap = 0;         /* capacity of vals[] */
static seq_t *vals = NULL;          /* stored sequence terms (a(1) at vals[0]) */
static size_t vals_len = 0;         /* number of terms stored so far */

/* state for generating rows */
static idx_t used_next = 1;          /* next candidate for smallest unused */
static idx_t next_row_len = 1;       /* number of terms in the next row to generate */
static seq_t prev_row_sum = 0;       /* sum of the previous row */

/* Ensure the bitset can hold indices up to |need| (inclusive). */
static inline void ensure_used(idx_t need)
{
    if (need <= used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, (new_cap + 1) * sizeof(uint8_t));
    if (!tmp) abort();                           /* allocation failure */
    memset(tmp + used_cap + 1, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = tmp;
    used_cap = new_cap;
}

/* Ensure the array of terms can hold at least |need| elements. */
static inline void ensure_vals(size_t need)
{
    if (need <= vals_cap) return;
    size_t new_cap = vals_cap ? vals_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    vals = tmp;
    vals_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety; sequence starts at a(1) */

    /* Generate terms until we have at least n of them. */
    while (vals_len < (size_t)n) {
        /* Handle the first row separately. */
        if (next_row_len == 1) {
            ensure_used(1);
            used[1] = 1;
            ensure_vals(vals_len + 1);
            vals[vals_len++] = (seq_t)1;
            prev_row_sum = (seq_t)1;
            next_row_len = 2;
            used_next = 2;
            continue;
        }

        /* ---- Construct the next row ---- */
        seq_t sum_prefix = 0;

        /* take the first (next_row_len‑1) smallest unused numbers */
        for (idx_t i = 0; i < next_row_len - 1; ++i) {
            while (used_next <= used_cap && used[used_next]) ++used_next;
            ensure_used(used_next);
            seq_t term = (seq_t)used_next;
            used[used_next] = 1;
            ++used_next;
            ensure_vals(vals_len + 1);
            vals[vals_len++] = term;
            sum_prefix += term;
        }

        /* determine the last number of the row */
        seq_t last;
        {
            seq_t rem = sum_prefix % prev_row_sum;
            seq_t residue = (prev_row_sum - rem) % prev_row_sum;
            if (residue == 0) residue = prev_row_sum;   /* smallest positive multiple */

            seq_t candidate = residue;
            /* search for the smallest unused candidate congruent to residue modulo prev_row_sum */
            while (1) {
                idx_t cand_idx = (idx_t)candidate;   /* candidate fits in idx_t assumed */
                ensure_used(cand_idx);
                if (!used[cand_idx]) break;
                candidate += prev_row_sum;
            }
            last = candidate;
        }

        /* mark the last number as used */
        idx_t last_idx = (idx_t)last;
        ensure_used(last_idx);
        used[last_idx] = 1;

        /* record the last term */
        ensure_vals(vals_len + 1);
        vals[vals_len++] = last;

        /* update state for the next row */
        prev_row_sum = sum_prefix + last;
        ++next_row_len;

        /* advance used_next to the next unused number */
        while (used_next <= used_cap && used[used_next]) ++used_next;
    }

    return vals[n - 1];
}
