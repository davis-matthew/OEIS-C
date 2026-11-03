/*
 * A054034  Numbers n such that n^2 contains exactly 6 different digits.
 * https://oeis.org/A054034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8984 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054034: Numbers n such that n^2 contains exactly 6 different digits.
   The function performs an on‑demand search of all candidates, storing the
   valid n values in a static array.  It is O(current_value) and is fast
   enough for n up to at least 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static inline int distinct_digits_count(seq_t v) {
    int mask = 0;
    int count = 0;
    while (v > 0) {
        int d = (int)(v % 10);
        int bit = 1 << d;
        if (!(mask & bit)) {
            mask |= bit;
            ++count;
            if (count > 6) return count; /* early exit */
        }
        v /= 10;
    }
    return count;
}

seq_t a(idx_t n) {
    /* Convert n (1‑based index) to the nth term. */
    static seq_t *valid = NULL;
    static size_t vm_len = 0, vm_cap = 0;
    static idx_t last_checked = 0; /* last candidate examined (≥0) */

    while (vm_len < n) {
        seq_t x = (seq_t)(last_checked + 1);
        seq_t sq = x * x;
        if (distinct_digits_count(sq) == 6) {
            if (vm_len == vm_cap) {
                size_t new_cap = vm_cap ? vm_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, valid, new_cap);
                if (!tmp) abort(); /* memory allocation failed */
                valid = tmp;
                vm_cap = new_cap;
            }
            valid[vm_len++] = x;
        }
        ++last_checked;
    }
    return valid[n - 1];
}
