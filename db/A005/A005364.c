/*
 * A005364  Hoggatt sequence with parameter d=6.
 * https://oeis.org/A005364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{h=0..n-1} Product_{k=0..h}
 *            binomial(n+5-k,6)/binomial(6+k,6), where each ratio is
 *            Product_{j=0..5} (n-k+j)/(k+1+j); partial products are integers
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   a005364_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005364_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t p = 1;
    unsigned long long k;
    int i, j;

    for (k = 0; k < (unsigned long long)n; k++) {
        unsigned long long num[6], den[6];
        for (j = 0; j < 6; j++) {
            num[j] = (unsigned long long)n - k + (unsigned long long)j;
            den[j] = k + 1 + (unsigned long long)j;
        }
        /* cancel each denominator factor against numerators and p */
        for (i = 0; i < 6; i++) {
            unsigned long long d = den[i];
            for (j = 0; j < 6 && d > 1; j++) {
                unsigned long long g = a005364_gcd(d, num[j]);
                d /= g;
                num[j] /= g;
            }
            if (d > 1) {
                seq_t sd = (seq_t)d;
                if (sd != 0) {
                    unsigned long long r = (unsigned long long)(p % sd);
                    unsigned long long g = a005364_gcd(d, r);
                    seq_t sg = (seq_t)g;
                    if (g > 1 && sg != 0) p /= sg;
                }
            }
        }
        for (j = 0; j < 6; j++) {
            if (num[j] != 1) p *= (seq_t)num[j];
        }
        result += p;
    }
    return result;
}
