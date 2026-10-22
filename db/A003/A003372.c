/*
 * A003372  Numbers that are the sum of 5 positive 7th powers.
 * https://oeis.org/A003372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is the sum of 5 positive 7th powers
 * Range:     u8 none, u16 none, u32 n <= 2424, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cached_sums = NULL;
static idx_t cached_count = 0;

static int cmp_seq_t(const void *a, const void *b) {
    seq_t va = *(seq_t *)a;
    seq_t vb = *(seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void generate_sums(int limit) {
    seq_t *temp = (seq_t *)malloc(500000 * sizeof(seq_t));
    idx_t count = 0;
    
    for (int a = 1; a <= limit; a++) {
        seq_t a7 = (seq_t)a * a * a * a * a * a * a;
        for (int b = a; b <= limit; b++) {
            seq_t b7 = (seq_t)b * b * b * b * b * b * b;
            for (int c = b; c <= limit; c++) {
                seq_t c7 = (seq_t)c * c * c * c * c * c * c;
                for (int d = c; d <= limit; d++) {
                    seq_t d7 = (seq_t)d * d * d * d * d * d * d;
                    for (int e = d; e <= limit; e++) {
                        seq_t e7 = (seq_t)e * e * e * e * e * e * e;
                        temp[count++] = a7 + b7 + c7 + d7 + e7;
                    }
                }
            }
        }
    }
    
    qsort(temp, count, sizeof(seq_t), cmp_seq_t);
    
    seq_t *dedup = (seq_t *)malloc(count * sizeof(seq_t));
    idx_t dedup_count = 0;
    seq_t last = (seq_t)(-1);
    for (idx_t i = 0; i < count; i++) {
        if (temp[i] != last) {
            dedup[dedup_count++] = temp[i];
            last = temp[i];
        }
    }
    
    free(temp);
    if (cached_sums) free(cached_sums);
    cached_sums = dedup;
    cached_count = dedup_count;
}

seq_t a(idx_t n) {
    if (!cached_sums) {
        generate_sums(30);
    }
    if (n < 1 || n > cached_count) return 0;
    return cached_sums[n - 1];
}
