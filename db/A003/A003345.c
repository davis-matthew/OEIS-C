/*
 * A003345  Numbers that are the sum of 11 positive 4th powers.
 * https://oeis.org/A003345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum of 11 positive 4th powers, finding the n-th distinct sum in
 *            increasing order
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_seq_t_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compare_seq_t_local.h"

static seq_t *g_cache = NULL;
static idx_t g_cache_len = 0;

static void generate_sums(seq_t *sums, idx_t *count, int depth, int last_base, seq_t current_sum, int max_base) {
    if (depth == 11) {
        if (*count < 1000000) {
            sums[*count] = current_sum;
            (*count)++;
        }
        return;
    }
    for (int base = last_base; base <= max_base; base++) {
        seq_t base2 = (seq_t)base * base;
        seq_t base4 = base2 * base2;
        generate_sums(sums, count, depth + 1, base, current_sum + base4, max_base);
    }
}

seq_t a(idx_t n) {
    if (g_cache == NULL) {
        seq_t *sums = (seq_t *)malloc(1000000 * sizeof(seq_t));
        if (sums == NULL) return 0;
        
        idx_t count = 0;
        int max_base = 12;
        
        generate_sums(sums, &count, 0, 1, 0, max_base);
        
        qsort(sums, count, sizeof(seq_t), compare_seq_t_local);
        
        idx_t unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || sums[i] != sums[i - 1]) {
                sums[unique_count++] = sums[i];
            }
        }
        
        g_cache = sums;
        g_cache_len = unique_count;
    }
    
    if (n > 0 && n <= g_cache_len) {
        return g_cache[n - 1];
    }
    return 0;
}
