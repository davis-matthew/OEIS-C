/*
 * A007336  Signature sequence of sqrt 2 (arrange the numbers i+j*x (i,j >= 1)
 *          in increasing order; the sequence of i's is the signature of x).
 * https://oeis.org/A007336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(n) = i from the n-th pair (i,j) arranged by i + j*sqrt(2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


typedef struct {
    unsigned int i;
    unsigned int j;
} Pair;

static int cmp_pairs(const void *a, const void *b) {
    const Pair *p1 = (const Pair *)a;
    const Pair *p2 = (const Pair *)b;
    
    double v1 = p1->i + p1->j * 1.41421356237309504880;
    double v2 = p2->i + p2->j * 1.41421356237309504880;
    
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    unsigned int limit = 500;
    while ((unsigned long long)limit * limit < (unsigned long long)n * 4ULL) {
        limit += 100;
    }
    if (limit > 10000) limit = 10000;
    
    size_t capacity = (size_t)limit * (size_t)limit;
    Pair *pairs = (Pair *)malloc(capacity * sizeof(Pair));
    
    size_t count = 0;
    for (unsigned int i = 1; i <= limit; i++) {
        for (unsigned int j = 1; j <= limit; j++) {
            pairs[count].i = i;
            pairs[count].j = j;
            count++;
        }
    }
    
    qsort(pairs, count, sizeof(Pair), cmp_pairs);
    
    seq_t result = (seq_t)pairs[n - 1].i;
    free(pairs);
    
    return result;
}
