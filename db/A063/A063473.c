/*
 * A063473  a(n) = M(2*n-1), where M(n) is Mertens's function (A002321):
 *          Sum_{k=1..n} mu(k), where mu = Moebius function (A008683).
 * https://oeis.org/A063473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, changed
 * Formula:   a(n) = M(2*n-1) where M(n) is the Mertens function * M(n) =
 *            Σ_{k=1..n} μ(k), μ = Moebius function
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence starts at n=1 */

    idx_t target = 2 * n - 1;            /* we need M(target) */

    /* allocate arrays for the linear sieve */
    idx_t size = target + 1;
    unsigned char *is_comp = (unsigned char *)calloc(size, sizeof(unsigned char));
    if (!is_comp) return 0;              /* allocation failed */

    seq_t *mu      = (seq_t *)calloc(size, sizeof(seq_t));
    if (!mu) { free(is_comp); return 0; }

    /* primes array: enough space for all primes up to target
       (<= target/2 for target>3) */
    idx_t *primes = (idx_t *)malloc((size_t)(size / 2 + 1) * sizeof(idx_t));
    if (!primes) { free(is_comp); free(mu); return 0; }

    size_t prime_cnt = 0;

    /* μ(1) = 1 */
    mu[1] = 1;

    /* Linear sieve for μ up to target */
    for (idx_t i = 2; i <= target; ++i) {
        if (!is_comp[i]) {               /* i is prime */
            primes[prime_cnt++] = i;
            mu[i] = (seq_t)-1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p   = primes[j];
            idx_t ip  = i * p;
            if (ip > target) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = (seq_t)-mu[i];
            }
        }
    }

    /* Compute M(target) = Σ_{k=1..target} μ(k) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= target; ++i)
        sum += mu[i];

    /* free temporary memory */
    free(is_comp);
    free(mu);
    free(primes);

    return sum;
}
