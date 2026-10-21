/*
 * A237349  a(n) = Sum_{i=1..n} ( Product_{k|i} d(k) ), where d(n) =
 *          A000005(n).
 * https://oeis.org/A237349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} ( Product_{k|i} d(k) ), where d(k) is the
 *            number of divisors of k (A000005)
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for n >= 1.
 * Uses a simple sieve to build smallest prime factors (spf) up to n,
 * then computes d(k) for all k <= n, and finally sums the product of
 * d(k) over all divisors k of each i. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for callers that might pass 0 */

    /* Allocate array for smallest prime factors. */
    idx_t *spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf) abort();              /* allocation failure – unlikely for n ≤ 10000 */

    /* Initialise spf array. */
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Allocate array for divisor counts d[0..n].  Use seq_t since d fits in
       small values but we keep the same type throughout. */
    seq_t *d = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!d) { free(spf); abort(); }

    d[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t res = 1;
        idx_t x = i;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            res = res * (seq_t)(e + 1);   /* contribute (e+1) to divisor count */
        }
        d[i] = res;
    }

    /* Compute the sequence values and sum them. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = 1;
        for (idx_t k = 1; k * k <= i; ++k) {
            if (i % k == 0) {
                prod = prod * d[k];
                idx_t other = i / k;
                if (other != k) prod = prod * d[other];
            }
        }
        sum += prod;
    }

    free(spf);
    free(d);
    return sum;
}
