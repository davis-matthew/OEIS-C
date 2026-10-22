/*
 * A002746  Sum of logarithmic numbers.
 * https://oeis.org/A002746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A000005(k)*(k-1)!*binomial(n, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   a002746_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a002746_gcd.h"

seq_t a(idx_t n) {
    idx_t k, m;
    int *tau = (int *)calloc((size_t)n + 1, sizeof(int));
    for (k = 1; k <= n; k++)
        for (m = k; m <= n; m += k)
            tau[m]++;

    seq_t result = 0;
    seq_t fact = 1;   /* (k-1)! */
    seq_t C = 1;      /* binomial(n, k-1) */
    for (k = 1; k <= n; k++) {
        /* C(n,k) = C(n,k-1) * (n-k+1) / k, done without overflowing */
        unsigned long long r = (unsigned long long)(C % k);
        unsigned long long g = a002746_gcd(r, (unsigned long long)k);
        idx_t kk = k / (idx_t)g;
        C = (C / (seq_t)g) * (seq_t)((n - k + 1) / kk);

        result += (seq_t)tau[k] * fact * C;
        fact *= (seq_t)k;
    }
    free(tau);
    return result;
}
