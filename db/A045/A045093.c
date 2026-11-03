/*
 * A045093  Numbers whose base-4 representation contains exactly one 1 and
 *          three 2's.
 * https://oeis.org/A045093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑4 representation contains exactly one '1' and three '2's. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has_full_count(idx_t x) {
    int c1 = 0, c2 = 0, digit;
    while (x) {
        digit = (int)(x % 4);
        x /= 4;
        if (digit == 1) ++c1;
        else if (digit == 2) ++c2;
    }
    return (c1 == 1 && c2 == 3);
}

static bool matches_idx(idx_t x) {
    int c1 = 0, c2 = 0, digit;
    while (x) {
        digit = (int)(x % 4);
        x /= 4;
        if (digit == 1) ++c1;
        else if (digit == 2) ++c2;
        else if (digit != 0 && digit != 3) return false; /* impossible, but keep safety */
    }
    return (c1 == 1 && c2 == 3);
}

static bool matches(idx_t x) {
    return has_full_count(x);
}

seq_t a(idx_t n) {
    /* The sequence is stored in increasing order. We grow the
       cache only when a higher index is requested. */
    static seq_t *values = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t next_checked = 0;

    if (n == 0) return 0; /* undefined, but safe */

    if ((size_t)n <= sz) {
        return values[(size_t)n - 1];
    }

    if (!values) {
        cap = 16;
        values = mem_new(seq_t, cap);
    }

    while (sz < (size_t)n) {
        ++next_checked;
        if (matches(next_checked)) {
            if (sz == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, values, new_cap);
                if (!tmp) { /* allocation failure, abort */
                    mem_free(values);
                    exit(1);
                }
                values = tmp;
                cap = new_cap;
            }
            values[sz++] = (seq_t)next_checked;
        }
    }
    return values[(size_t)n - 1];
}
