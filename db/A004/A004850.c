/*
 * A004850  Numbers that are the sum of at most 10 positive 5th powers.
 * https://oeis.org/A004850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: a(n) is the n-th number x >= 0 whose minimal count of
 *            positive 5th powers summing to x is <= 10 (DP over min counts)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static unsigned long long *vals = NULL;
    static idx_t vals_len = 0;
    static unsigned long long limit = 4096;

    while (n > vals_len) {
        unsigned long long L = limit;
        unsigned char *mc = (unsigned char *)malloc((size_t)(L + 1));
        unsigned long long pw[64];
        int np = 0;
        unsigned long long i, x;
        int k;

        for (i = 1;; i++) {
            unsigned long long p = i * i * i * i * i;
            if (p > L) break;
            pw[np++] = p;
        }

        mc[0] = 0;
        for (x = 1; x <= L; x++) {
            unsigned char best = 11;
            for (k = 0; k < np && pw[k] <= x; k++) {
                unsigned char c = (unsigned char)(mc[x - pw[k]] + 1);
                if (c < best) best = c;
            }
            mc[x] = best;
        }

        idx_t cnt = 0;
        for (x = 0; x <= L; x++)
            if (mc[x] <= 10) cnt++;

        if (cnt >= n) {
            unsigned long long *nv = (unsigned long long *)malloc((size_t)cnt * sizeof(unsigned long long));
            idx_t j = 0;
            for (x = 0; x <= L; x++)
                if (mc[x] <= 10) nv[j++] = x;
            free(vals);
            vals = nv;
            vals_len = cnt;
        }
        free(mc);
        if (n > vals_len) limit *= 2;
    }

    return (seq_t)vals[n - 1];
}
