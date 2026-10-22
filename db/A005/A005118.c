/*
 * A005118  Number of simple allowable sequences on 1..n containing the
 *          permutation 12...n.
 * https://oeis.org/A005118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n,2)! / (1^(n-1) * 3^(n-2) * ... * (2n-3)^1), evaluated
 *            via prime factorization (Legendre exponents of m! minus
 *            exponents of the odd-number product), so no intermediate exceeds
 *            the result
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n <= 2) return 1;

    uint64_t m = (uint64_t)n * (uint64_t)(n - 1) / 2;
    uint64_t lim = 2 * (uint64_t)n;

    long long *dexp = (long long *)calloc((size_t)lim + 1, sizeof(long long));
    for (uint64_t k = 0; k + 2 <= (uint64_t)n; k++) {
        uint64_t q = 2 * k + 1;
        long long e = (long long)((uint64_t)n - 1 - k);
        uint64_t r = q;
        for (uint64_t p = 3; p * p <= r; p += 2) {
            while (r % p == 0) {
                dexp[p] += e;
                r /= p;
            }
        }
        if (r > 1) dexp[r] += e;
    }

    unsigned char *comp = (unsigned char *)calloc((size_t)m + 1, 1);
    seq_t result = 1;
    uint64_t acc = 1;
    for (uint64_t p = 2; p <= m; p++) {
        if (comp[p]) continue;
        if (p * p <= m)
            for (uint64_t j = p * p; j <= m; j += p) comp[j] = 1;

        long long e = 0;
        uint64_t t = m;
        while (t) {
            t /= p;
            e += (long long)t;
        }
        if (p <= lim) e -= dexp[p];

        for (long long i = 0; i < e; i++) {
            if (acc > (UINT64_C(1) << 32) / p) {
                result *= (seq_t)acc;
                acc = 1;
            }
            acc *= p;
        }
    }
    result *= (seq_t)acc;

    free(comp);
    free(dexp);
    return result;
}
