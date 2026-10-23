/*
 * A015622  Quadruples of different integers from [ 1,n ] with no global
 *          factor.
 * https://oeis.org/A015622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} μ(d) * C(floor(n/d), 4)
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(d) is the Möbius function, C(k,4)=k*(k-1)*(k-2)*(k-3)/24 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    /* linear sieve for μ up to n */
    int *mu = (int *)malloc((n + 1) * sizeof(int));
    unsigned char *comp = (unsigned char *)calloc((n + 1), sizeof(unsigned char));
    int *primes = (int *)malloc((n + 1) * sizeof(int));
    int pcnt = 0;
    mu[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (!comp[i]) {
            primes[pcnt++] = (int)i;
            mu[(int)i] = -1;
        }
        for (int j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > n)
                break;
            comp[ip] = 1;
            if (i % p == 0) {
                mu[(int)ip] = 0;
                break;
            } else {
                mu[(int)ip] = -mu[(int)i];
            }
        }
    }

    /* accumulate the sum using a signed long long accumulator */
    long long signed_res = 0;

    for (idx_t d = 1; d <= n; ++d) {
        int mud = mu[(int)d];
        if (mud == 0)
            continue;
        idx_t m = n / d;
        if (m < 4)
            continue;
        /* C(m,4) fits in 64-bit for n up to at least 10^4 */
        long long comb = (long long)m * (m - 1) * (m - 2) * (m - 3) / 24;
        if (mud > 0)
            signed_res += comb;
        else
            signed_res -= comb;
    }

    free(mu);
    free(comp);
    free(primes);

    return (seq_t)signed_res;
}
