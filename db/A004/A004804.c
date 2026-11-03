/*
 * A004804  Numbers that are the sum of 4 nonzero 10th powers.
 * https://oeis.org/A004804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sums of 4 nonzero 10th powers in ascending order, each in
 *            nondecreasing order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 8480 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


static seq_t *cached = NULL;
static idx_t cached_count = 0;

static int cmp_sort(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void generate(void) {
    if (cached != NULL) return;
    
    idx_t base = 50;
    
    // Precompute 10th powers
    seq_t *powers = mem_new(seq_t, (base + 1));
    for (idx_t i = 1; i <= base; i++) {
        seq_t x = (seq_t)i;
        seq_t p = x;
        for (int j = 1; j < 10; j++) {
            p = p * x;
        }
        powers[i] = p;
    }
    
    // Generate sums
    seq_t *temp = mem_new(seq_t, 500000);
    idx_t count = 0;
    
    for (idx_t a = 1; a <= base; a++) {
        for (idx_t b = a; b <= base; b++) {
            for (idx_t c = b; c <= base; c++) {
                for (idx_t d = c; d <= base; d++) {
                    temp[count++] = powers[a] + powers[b] + powers[c] + powers[d];
                }
            }
        }
    }
    
    mem_free(powers);
    
    // Sort
    mem_qsort(temp, count, sizeof(seq_t), cmp_sort);
    
    // Remove duplicates
    idx_t out_idx = 0;
    for (idx_t i = 0; i < count; i++) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[out_idx++] = temp[i];
        }
    }
    
    cached = temp;
    cached_count = out_idx;
}

seq_t a(idx_t n) {
    generate();
    return cached[n - 1];
}
