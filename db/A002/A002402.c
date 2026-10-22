/*
 * A002402  Coefficients for step-by-step integration.
 * https://oeis.org/A002402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{j=0..n-1} |Stirling1(n,j+1)| * lcm(1..n+1) /
 *            ((j+1)*(j+2)), i.e. a(n) = n! * lcm(1..n+1) * Sum_{k=1..n}
 *            k*|gamma*_k|, where gamma*_k are the Adams-Moulton coefficients
 *            (the coefficient of f at the second node of the (n+1)-point
 *            Adams-Moulton formula, over the common denominator n!...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a002402_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002402_isprime.h"

seq_t a(idx_t n) {
    idx_t N = n + 1;
    idx_t m, j, k;
    seq_t L = (seq_t)1;
    seq_t sum = (seq_t)0;
    seq_t *q;

    /* L = lcm(1..n+1) */
    for (m = 2; m <= N; m++) {
        if (a002402_isprime(m)) {
            idx_t pp = m;
            while (pp <= N / m) pp *= m;
            L *= (seq_t)pp;
        }
    }

    /* q[j] = coefficient of t^j in (t+1)(t+2)...(t+n-1) = |Stirling1(n, j+1)| */
    q = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    for (j = 0; j < n; j++) q[j] = (seq_t)0;
    q[0] = (seq_t)1;
    for (k = 1; k < n; k++) {
        for (j = k; j >= 1; j--)
            q[j] = q[j] * (seq_t)k + q[j - 1];
        q[0] = q[0] * (seq_t)k;
    }

    for (j = 0; j < n; j++) {
        seq_t d = (seq_t)((j + 1) * (j + 2));
        if (d == 0) continue; /* only possible when the result overflows seq_t */
        sum += q[j] * (L / d);
    }
    free(q);
    return sum * (seq_t)n;
}
