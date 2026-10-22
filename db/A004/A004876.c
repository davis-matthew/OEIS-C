/*
 * A004876  Numbers that are the sum of at most 3 nonzero 8th powers.
 * https://oeis.org/A004876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 795, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Generate all numbers that are sums of at most 3 nonzero 8th powers, sort, and return the n-th one */

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static seq_t pow8(seq_t x) {
    seq_t result = x;
    for (int i = 0; i < 7; i++) result = result * x;
    return result;
}

static void ensure_generated(idx_t n) {
    if (memo_len >= n) return;
    
    // Estimate k_max: we need roughly k_max^3/6 terms
    idx_t k_max = 20;
    while (k_max * k_max * k_max < 6 * (n + 100)) {
        k_max += 10;
    }
    
    // Allocate buffer
    idx_t max_size = 2 + k_max + k_max * k_max + k_max * k_max * k_max;
    seq_t *buf = (seq_t *)malloc(max_size * sizeof(seq_t));
    
    // Store 8th powers
    seq_t *powers = (seq_t *)malloc((k_max + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= k_max; i++) {
        powers[i] = pow8((seq_t)i);
    }
    
    idx_t cnt = 0;
    buf[cnt++] = 0;
    
    // Single powers
    for (idx_t i = 1; i <= k_max; i++) {
        buf[cnt++] = powers[i];
    }
    
    // Pairs of powers
    for (idx_t i = 1; i <= k_max; i++) {
        for (idx_t j = i; j <= k_max; j++) {
            buf[cnt++] = powers[i] + powers[j];
        }
    }
    
    // Triples of powers
    for (idx_t i = 1; i <= k_max; i++) {
        for (idx_t j = i; j <= k_max; j++) {
            for (idx_t k = j; k <= k_max; k++) {
                buf[cnt++] = powers[i] + powers[j] + powers[k];
            }
        }
    }
    
    // Sort
    qsort(buf, cnt, sizeof(seq_t), cmp_seq);
    
    // Deduplicate
    idx_t dedup = 0;
    for (idx_t i = 0; i < cnt; i++) {
        if (dedup == 0 || buf[dedup - 1] != buf[i]) {
            buf[dedup++] = buf[i];
        }
    }
    
    if (memo != NULL) free(memo);
    memo = buf;
    memo_len = dedup;
    free(powers);
}

seq_t a(idx_t n) {
    ensure_generated(n);
    if (n > memo_len) ensure_generated(n * 2);
    return memo[n - 1];
}
