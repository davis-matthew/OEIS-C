/*
 * A005156  Number of alternating sign 2n+1 X 2n+1 matrices symmetric about
 *          the vertical axis (VSASM's); also 2n X 2n off-diagonally symmetric
 *          alternating sign matrices (OSASM's).
 * https://oeis.org/A005156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/2^n) * Product_{k=1..n}
 *            ((6k-2)!(2k-1)!)/((4k-1)!(4k-2)!) (Razumov/Stroganov); evaluated
 *            via prime-exponent counting
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   a000248_pow, cnt_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a000248_pow.h"
#include "../utils/cnt_ge.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    long long nn = (long long)n;
    long long N = 6 * nn;
    int *spf = (int *)calloc((size_t)(N + 1), sizeof(int));
    long long *ex = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (long long i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (long long j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (int)i;
        }
    }
    for (long long j = 2; j <= N; j++) {
        long long c = cnt_ge(nn, 6, 2, j) + cnt_ge(nn, 2, 1, j)
                    - cnt_ge(nn, 4, 1, j) - cnt_ge(nn, 4, 2, j);
        if (c == 0) continue;
        long long m = j;
        while (m > 1) {
            long long p = spf[m];
            m /= p;
            ex[p] += c;
        }
    }
    ex[2] -= nn;
    seq_t result = 1;
    for (long long p = 2; p <= N; p++) {
        if (spf[p] == p && ex[p] > 0)
            result *= a000248_pow((seq_t)p, (idx_t)ex[p]);
    }
    free(spf);
    free(ex);
    return result;
}
