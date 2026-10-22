/*
 * A007337  Signature sequence of sqrt(3).
 * https://oeis.org/A007337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sort i + j*sqrt(3) (i,j >= 1) in increasing order and list the
 *            i's. The terms are grouped into bands [k, k+1) for k = 2, 3,
 *            ...: band k holds the pairs i = k - floor(j*sqrt(3)) for every j
 *            with j*sqrt(3) < k, ordered by frac(j*sqrt(3)) ascending. Every
 *            comparison is done in exact integer arithmetic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sig3_fl, sig3_isqrt, sig3_less
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sig3_fl.h"
#include "../utils/sig3_isqrt.h"
#include "../utils/sig3_less.h"

seq_t a(idx_t n) {
    long long cap = 64, J = 0;
    long long *ord = (long long *)malloc((size_t)cap * sizeof(long long));
    idx_t count = 0;
    long long k;
    for (k = 1;; k++) {
        long long t;
        while (3 * (J + 1) * (J + 1) < k * k) {
            long long j = J + 1, p;
            if (J + 1 > cap) {
                cap *= 2;
                ord = (long long *)realloc(ord, (size_t)cap * sizeof(long long));
            }
            p = J;
            while (p > 0 && sig3_less(j, ord[p - 1])) {
                ord[p] = ord[p - 1];
                p--;
            }
            ord[p] = j;
            J++;
        }
        for (t = 0; t < J; t++) {
            count++;
            if (count == n) {
                long long i = k - sig3_fl(ord[t]);
                free(ord);
                return (seq_t)i;
            }
        }
    }
}
