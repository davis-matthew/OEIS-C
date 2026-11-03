/*
 * A037985  Numbers whose maximal base 6 run length is 2.
 * https://oeis.org/A037985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose maximal base-6 run length is exactly 2 */

/* This function returns the n-th integer (n ≥ 1) whose maximum run
 * length of any digit in its base‑6 representation equals 2.
 * The sequence is generated incrementally and cached so that
 * subsequent calls are fast regardless of the order of n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has_run_length_two(seq_t val) {
    seq_t temp = val;
    int last_digit = -1;
    int cur_count = 0;
    int max_count = 0;

    while (temp > (seq_t)0) {
        int digit = (int)(temp % (seq_t)6);
        if (digit == last_digit) {
            ++cur_count;
        } else {
            cur_count = 1;
            last_digit = digit;
        }
        if (cur_count > max_count) max_count = cur_count;
        temp /= (seq_t)6;
    }
    return (max_count == 2);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t size = 0;
    static seq_t candidate = 0;

    if (n == 0) return 0;                 /* undefined but harmless */

    if (cache == NULL) {
        capacity = 64;
        cache = mem_new(seq_t, capacity);
        if (!cache) abort();
    }

    while (size < n) {
        ++candidate;
        if (has_run_length_two(candidate)) {
            if (size == capacity) {
                capacity *= 2;
                seq_t *newp = mem_resize(seq_t, cache, capacity);
                if (!newp) abort();
                cache = newp;
            }
            cache[size++] = candidate;
        }
    }
    return cache[n - 1];
}
