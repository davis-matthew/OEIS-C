/*
 * A076224  Counts of the minimum value in n-th row of A076221.
 * https://oeis.org/A076224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in 1..n with T(n,k) = min_j T(n,j), where
 *            T(n,k) = #{x<=n : gcd(x,k)=1, x!=k} (A076221), computed by
 *            inclusion-exclusion over the squarefree divisors of k
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    long long N = (long long)n;
    unsigned *spf = (unsigned *)malloc((size_t)(N + 1) * sizeof(unsigned));
    long long i, j;
    for (i = 0; i <= N; i++) spf[i] = 0;
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (unsigned)i;
        }
    }
    long long best = -1, cnt = 0;
    for (long long k = 1; k <= N; k++) {
        long long val;
        if (k == 1) {
            val = N - 1;
        } else {
            long long ps[16];
            int np = 0;
            long long m = k;
            while (m > 1) {
                long long p = spf[m];
                ps[np++] = p;
                while (m % p == 0) m /= p;
            }
            val = 0;
            for (int mask = 0; mask < (1 << np); mask++) {
                long long d = 1;
                int bits = 0;
                for (int b = 0; b < np; b++)
                    if (mask & (1 << b)) { d *= ps[b]; bits++; }
                if (bits & 1) val -= N / d;
                else val += N / d;
            }
        }
        if (best < 0 || val < best) { best = val; cnt = 1; }
        else if (val == best) cnt++;
    }
    free(spf);
    return (seq_t)cnt;
}
