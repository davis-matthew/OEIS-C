/*
 * A232172  Partial sums of second arithmetic derivative of natural numbers.
 * https://oeis.org/A232172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} i'' (second arithmetic derivative summed)
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Choose a sieve limit comfortably larger than any D(i) for i≤n
       (n·20 + 200 is sufficient for n ≤ 10000). */
    idx_t limit = n * 20 + 200;
    if (limit < n)
        limit = n;

    /* Allocate smallest‑prime‑factor array and derivative array. */
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    seq_t *D   = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!spf || !D) {
        free(spf);
        free(D);
        return (seq_t)0;   /* out of memory – return 0, caller can decide */
    }

    /* Smallest prime factor sieve. */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] > i)
                    spf[j] = i;
        }
    }

    /* Arithmetic derivative D(1)=0, D(p)=1 for prime p,
       D(ab)=a D(b)+b D(a). */
    D[0] = 0;   /* convenient for D(0) in second derivative of 1 */
    D[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        D[i] = (seq_t)m + (seq_t)p * D[m];
    }

    /* Sum second derivatives i'' = D(D(i)) for i=1..n. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t di = (idx_t)D[i];          /* D(i) fits in idx_t for n≤10000 */
        if (di <= limit)                /* safety check */
            sum += D[di];
    }

    free(spf);
    free(D);
    return sum;
}
