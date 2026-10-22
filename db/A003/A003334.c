/*
 * A003334  Numbers that are the sum of 11 positive cubes.
 * https://oeis.org/A003334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer that is the sum of 11 positive
 *            cubes. For n > 229: a(n) = n + 92. For n <= 229: computed by
 *            generating all sums of 11 positive cubes up to 321
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_sums_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/generate_sums_rec.h"

static seq_t *cached_sums = NULL;
static idx_t cached_count = 0;
static int computed = 0;

static int cmp_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void compute_sums(void) {
    if (computed) return;
    computed = 1;
    
    seq_t *temp = (seq_t *)malloc(10000 * sizeof(seq_t));
    idx_t count = 0;
    
    generate_sums_rec(11, (seq_t)1, (seq_t)0, (seq_t)321, temp, &count);
    
    qsort(temp, (size_t)count, sizeof(seq_t), cmp_seq_t);
    
    seq_t *unique = (seq_t *)malloc((size_t)count * sizeof(seq_t));
    idx_t unique_count = 0;
    
    for (idx_t i = 0; i < count; i++) {
        if (i == 0 || temp[i] != temp[i-1]) {
            unique[unique_count++] = temp[i];
        }
    }
    
    free(temp);
    
    cached_sums = unique;
    cached_count = unique_count;
}

seq_t a(idx_t n) {
    if (n > 229) {
        return ((seq_t)n) + 92;
    }
    
    compute_sums();
    return cached_sums[n - 1];
}
