/*
 * A003371  Numbers that are the sum of 4 positive 7th powers.
 * https://oeis.org/A003371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th number that is a sum of 4 positive 7th powers
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 9773 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int cmp_seq_t(const void *a, const void *b) {
	seq_t va = *(const seq_t *)a;
	seq_t vb = *(const seq_t *)b;
	if (va < vb) return -1;
	if (va > vb) return 1;
	return 0;
}

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;

static void build_cache(void) {
    idx_t max_base = 60;
    seq_t *all = mem_new(seq_t, 700000);
    idx_t cnt = 0;
    
    seq_t *pow7 = mem_new(seq_t, (max_base + 1));
    for (idx_t i = 0; i <= max_base; i++) {
        seq_t x = (seq_t)i;
        pow7[i] = x * x * x * x * x * x * x;
    }
    
    for (idx_t a = 1; a <= max_base; a++) {
        for (idx_t b = a; b <= max_base; b++) {
            for (idx_t c = b; c <= max_base; c++) {
                for (idx_t d = c; d <= max_base; d++) {
                    all[cnt++] = pow7[a] + pow7[b] + pow7[c] + pow7[d];
                }
            }
        }
    }
    
    mem_free(pow7);
    mem_qsort(all, cnt, sizeof(seq_t), cmp_seq_t);
    
    idx_t write_pos = 0;
    for (idx_t read_pos = 0; read_pos < cnt; read_pos++) {
        if (write_pos == 0 || all[read_pos] != all[write_pos - 1]) {
            all[write_pos++] = all[read_pos];
        }
    }
    
    seq_cache = all;
    cache_len = write_pos;
}

seq_t a(idx_t n) {
    if (seq_cache == NULL) {
        build_cache();
    }
    return (n <= cache_len) ? seq_cache[n - 1] : 0;
}
