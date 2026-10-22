/*
 * A002088  Sum of totient function: a(n) = Sum_{k=1..n} phi(k), cf. A000010.
 * https://oeis.org/A002088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} phi(k), with phi computed by a totient sieve
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    uint64_t *t;
    idx_t i, j;
    if (n == 0) return result;
    t = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!t) return result;
    for (i = 0; i <= n; i++) t[i] = i;
    for (i = 2; i <= n; i++) {
        if (t[i] == i) {
            for (j = i; j <= n; j += i) t[j] -= t[j] / i;
        }
    }
    for (i = 1; i <= n; i++) result += (seq_t)t[i];
    free(t);
    return result;
}
