/*
 * A001142  a(n) = Product_{k=1..n} k^(2k - 1 - n).
 * https://oeis.org/A001142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k^(2k-1-n), computed via prime
 *            factorization: v_p(a(n)) = Sum_{j>=1} (p^j*q*(q+1) - q*(n+1)), q
 *            = floor(n/p^j)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a001142_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001142_pw.h"

seq_t a(idx_t n) {
    if (n < 2) return 1;
    char *comp = (char *)calloc((size_t)n + 1, 1);
    seq_t res = 1;
    for (idx_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        for (idx_t m = p * p; m <= n && p <= n / p; m += p) comp[m] = 1;
        long long e = 0;
        for (idx_t pj = p; pj <= n; ) {
            long long q = (long long)(n / pj);
            e += (long long)pj * q * (q + 1) - q * (long long)(n + 1);
            if (pj > n / p) break;
            pj *= p;
        }
        if (e > 0) res *= a001142_pw((seq_t)p, (unsigned long long)e);
    }
    free(comp);
    return res;
}
