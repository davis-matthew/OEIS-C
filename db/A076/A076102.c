/*
 * A076102  Final members of groups in A076105.
 * https://oeis.org/A076102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A076105(n*(n+1)/2), the last entry of row n of the
 *            triangle A076105 (computed by direct construction of the rows)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static idx_t used_cap = 0;
static uint8_t *used = NULL;

static size_t vals_cap = 0;
static seq_t *vals = NULL;
static size_t vals_len = 0;

static idx_t used_next = 1;
static idx_t next_row_len = 1;
static seq_t prev_row_sum = 0;

static inline void ensure_used(idx_t need)
{
    if (need <= used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, (new_cap + 1) * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + used_cap + 1, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = tmp;
    used_cap = new_cap;
}

static inline void ensure_vals(size_t need)
{
    if (need <= vals_cap) return;
    size_t new_cap = vals_cap ? vals_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, vals, new_cap);
    if (!tmp) abort();
    vals = tmp;
    vals_cap = new_cap;
}

static seq_t A076105_a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    while (vals_len < (size_t)n) {
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

        seq_t sum_prefix = 0;

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

        seq_t last;
        {
            seq_t rem = sum_prefix % prev_row_sum;
            seq_t residue = (prev_row_sum - rem) % prev_row_sum;
            if (residue == 0) residue = prev_row_sum;

            seq_t candidate = residue;
            while (1) {
                idx_t cand_idx = (idx_t)candidate;
                ensure_used(cand_idx);
                if (!used[cand_idx]) break;
                candidate += prev_row_sum;
            }
            last = candidate;
        }

        idx_t last_idx = (idx_t)last;
        ensure_used(last_idx);
        used[last_idx] = 1;

        ensure_vals(vals_len + 1);
        vals[vals_len++] = last;

        prev_row_sum = sum_prefix + last;
        ++next_row_len;

        while (used_next <= used_cap && used[used_next]) ++used_next;
    }

    return vals[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A076105_a(n * (n + 1) / 2);
}
