/*
 * A002244  Numbers that are not the sum of 3 distinct nonzero triangular
 *          numbers.
 * https://oeis.org/A002244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   sieve: mark every T(i)+T(j)+T(k) with 1 <= i < j < k (T(m) =
 *            m*(m+1)/2) up to a fixed bound; a(n) is the n-th unmarked
 *            positive integer (finite sequence)
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define A2244_LIM 100000L

static long *a2244_terms = NULL;
static long a2244_cnt = 0;

static void a2244_build(void) {
    unsigned char *rep = (unsigned char *)calloc((size_t)A2244_LIM + 1, 1);
    long tri[1000];
    long nt = 0, m, i, j, k;
    for (m = 1;; m++) {
        long t = m * (m + 1) / 2;
        if (t > A2244_LIM) break;
        tri[nt++] = t;
    }
    for (i = 0; i < nt; i++) {
        for (j = i + 1; j < nt; j++) {
            long s2 = tri[i] + tri[j];
            if (s2 > A2244_LIM) break;
            for (k = j + 1; k < nt; k++) {
                long s3 = s2 + tri[k];
                if (s3 > A2244_LIM) break;
                rep[s3] = 1;
            }
        }
    }
    a2244_cnt = 0;
    for (m = 1; m <= A2244_LIM; m++)
        if (!rep[m]) a2244_cnt++;
    a2244_terms = (long *)malloc((size_t)(a2244_cnt + 1) * sizeof(long));
    a2244_cnt = 0;
    for (m = 1; m <= A2244_LIM; m++)
        if (!rep[m]) a2244_terms[a2244_cnt++] = m;
    free(rep);
}

seq_t a(idx_t n) {
    if (a2244_terms == NULL) a2244_build();
    if (n < 1 || (long long)n > (long long)a2244_cnt) return (seq_t)0;
    return (seq_t)a2244_terms[n - 1];
}
