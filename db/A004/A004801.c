/*
 * A004801  Sum of 12 positive 9th powers.
 * https://oeis.org/A004801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest sum of 12 positive 9th powers
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 888 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int cmp_seq_val(const void *a, const void *b) {
    seq_t va = *(seq_t *)a;
    seq_t vb = *(seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static seq_t ninth_power(idx_t x) {
    seq_t v = (seq_t)x;
    for (int i = 0; i < 8; i++) {
        v = v * (seq_t)x;
    }
    return v;
}

static void generate_sums_helper(seq_t **sums, idx_t *count, idx_t *capacity,
                                  idx_t pos, idx_t min_val, idx_t max_base,
                                  idx_t *tuple) {
    if (pos == 12) {
        seq_t sum = 0;
        for (int i = 0; i < 12; i++) {
            sum = sum + ninth_power(tuple[i]);
        }
        if (*count >= *capacity) {
            *capacity = (*capacity) * 2;
            *sums = mem_resize(seq_t, *sums, (*capacity * sizeof(seq_t)) / sizeof(seq_t));
        }
        (*sums)[(*count)++] = sum;
        return;
    }
    
    for (idx_t val = min_val; val <= max_base; val++) {
        tuple[pos] = val;
        generate_sums_helper(sums, count, capacity, pos + 1, val, max_base, tuple);
    }
}

seq_t a(idx_t n) {
    static seq_t *sums = NULL;
    static idx_t count = 0;
    static bool computed = false;
    
    if (!computed) {
        idx_t capacity = 20000;
        sums = mem_new(seq_t, capacity);
        
        idx_t tuple[12];
        idx_t max_base = 7;
        
        generate_sums_helper(&sums, &count, &capacity, 0, 1, max_base, tuple);
        
        mem_qsort(sums, (size_t)count, sizeof(seq_t), cmp_seq_val);
        
        idx_t write_pos = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || sums[i] != sums[i-1]) {
                sums[write_pos++] = sums[i];
            }
        }
        count = write_pos;
        
        computed = true;
    }
    
    if (n > count) {
        return 0;
    }
    
    return sums[n - 1];
}
