/*
 * A005161  Number of alternating sign 2n+1 X 2n+1 matrices symmetric with
 *          respect to both horizontal and vertical axes (VHSASM's).
 * https://oeis.org/A005161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A005156(floor(n/2)) * A051255(ceiling(n/2)), where
 *            A005156(m) = Product_{k=0..m-1}
 *            (3k+2)*(6k+3)!*(2k+1)!/((4k+2)!*(4k+3)!) and A051255(m) =
 *            Product_{k=0..m-1} (3k+1)*(6k)!*(2k)!/((4k)!*(4k+1)!); evaluated
 *            exactly via prime factorization of the factorial products
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   add_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_factors.h"

seq_t a(idx_t n) {
    long long f = (long long)(n / 2);
    long long c = (long long)((n + 1) / 2);
    long long N = 6 * c + 4;
    long long *cnt = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    long long *pe = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    int *spf = (int *)calloc((size_t)(N + 1), sizeof(int));
    long long i, j, k;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (int)i;
        }
    }

    for (k = 0; k < f; k++) {
        cnt[6 * k + 3]++;
        cnt[2 * k + 1]++;
        cnt[4 * k + 2]--;
        cnt[4 * k + 3]--;
        add_factors(pe, spf, 3 * k + 2, 1);
    }
    for (k = 0; k < c; k++) {
        cnt[6 * k]++;
        cnt[2 * k]++;
        cnt[4 * k]--;
        cnt[4 * k + 1]--;
        add_factors(pe, spf, 3 * k + 1, 1);
    }

    {
        long long run = 0;
        for (j = N; j >= 2; j--) {
            run += cnt[j];
            if (run != 0) add_factors(pe, spf, j, run);
        }
    }

    seq_t r = 1;
    uint64_t chunk = 1;
    for (i = 2; i <= N; i++) {
        long long e;
        for (e = 0; e < pe[i]; e++) {
            if (chunk * (uint64_t)i > 0xFFFFFFFFULL) {
                r = r * (seq_t)chunk;
                chunk = 1;
            }
            chunk *= (uint64_t)i;
        }
    }
    r = r * (seq_t)chunk;

    free(cnt);
    free(pe);
    free(spf);
    return r;
}
