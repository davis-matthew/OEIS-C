/*
 * A015627  Quadruples of different integers from [ 2,n ] with no global
 *          factor.
 * https://oeis.org/A015627
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d≥1} μ(d) * C(k_d, 4) * where k_1 = n-1 (numbers 2..n)
 *            * k_d = ⌊n/d⌋ for d > 1 (multiples of d in [2,n]). * μ is the
 *            Möbius function. * The sum runs over d such that k_d ≥ 4
 *            (otherwise C(k_d,4)=0)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Multiplicative function: choose 4 from k */
static inline seq_t binom4(seq_t k)
{
    if (k < 4)
        return 0;
    /* Use seq_t for intermediate products; k ≤ n ≤ 10000 < 2^64 */
    seq_t res = k;
    res *= (k - 1);
    res *= (k - 2);
    res *= (k - 3);
    res /= 24;           /* 4! = 24 */
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;       /* offset starts at 2 */

    /* Upper bound for d: when ⌊n/d⌋ ≥ 4  → d ≤ n/4.  
     * But we also need to include d = 1, so allocate up to n/2 for safety. */
    idx_t limit = n / 2;
    if (limit < 1)
        limit = 1;

    /* Allocate arrays for smallest prime factor (spf) and Möbius mu */
    uint64_t *spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    int *mu   = (int *)malloc((limit + 1) * sizeof(int));

    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = 0;     /* initialize */

    /* Simple sieve for least prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* Compute Möbius function using the spf array */
    mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        if (m % p == 0)
            mu[i] = 0;
        else
            mu[i] = -mu[m];
    }

    /* Accumulate the sum using signed 64‑bit integer */
    long long sum = 0;

    for (idx_t d = 1; d <= limit; ++d) {
        if (mu[d] == 0)
            continue;

        seq_t k = (d == 1) ? (n - 1) : (n / d);
        if (k < 4)
            continue;

        seq_t comb = binom4(k);
        sum += (long long)mu[d] * (long long)comb;
    }

    free(spf);
    free(mu);

    return (seq_t)sum;
}
