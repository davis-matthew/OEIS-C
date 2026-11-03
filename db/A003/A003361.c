/*
 * A003361  Numbers that are the sum of 5 positive 6th powers.
 * https://oeis.org/A003361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is the sum of exactly 5 positive 6th
 *            powers
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 981 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int compare_seq_t(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static seq_t *cached_sequence = NULL;
static idx_t cached_length = 0;

static void init_sequence(void) {
    if (cached_sequence != NULL) return;
    
    seq_t *sums = mem_new(seq_t, 600000);
    idx_t count = 0;
    
    seq_t *powers = mem_new(seq_t, 36);
    for (idx_t i = 0; i <= 35; i++) {
        if (i == 0) {
            powers[i] = 0;
        } else {
            seq_t p = (seq_t)i;
            powers[i] = p * p * p * p * p * p;
        }
    }
    
    for (idx_t a = 1; a <= 35; a++) {
        for (idx_t b = a; b <= 35; b++) {
            for (idx_t c = b; c <= 35; c++) {
                for (idx_t d = c; d <= 35; d++) {
                    for (idx_t e = d; e <= 35; e++) {
                        if (count < 600000) {
                            seq_t sum = powers[a] + powers[b] + powers[c] + powers[d] + powers[e];
                            sums[count++] = sum;
                        }
                    }
                }
            }
        }
    }
    
    mem_free(powers);
    
    mem_qsort(sums, count, sizeof(seq_t), compare_seq_t);
    
    seq_t *result = mem_new(seq_t, count);
    idx_t unique_count = 0;
    for (idx_t i = 0; i < count; i++) {
        if (unique_count == 0 || sums[i] != sums[i-1]) {
            result[unique_count++] = sums[i];
        }
    }
    
    mem_free(sums);
    
    cached_sequence = result;
    cached_length = unique_count;
}

seq_t a(idx_t n) {
    if (cached_sequence == NULL) {
        init_sequence();
    }
    
    if (n >= 1 && n <= cached_length) {
        return cached_sequence[n - 1];
    }
    
    return 0;
}
