/*
 * A015651  Number of ordered 5-tuples of integers from [ 2,n ] with no global
 *          factor.
 * https://oeis.org/A015651
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} μ(d) * C( f(d)+4 , 5 ) where μ is the Möbius
 *            function and f(d) = (n-1) if d = 1 f(d) = ⌊n/d⌋ if d ≥ 2 (count
 *            of multiples of d in [2,n])
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int *mu = NULL;        /* Möbius values up to the current maximum n   */
static idx_t mu_max = 0;      /* number of precomputed μ entries            */

/* Precompute the Möbius function up to max_n using a linear sieve.  */
static void compute_mu(idx_t max_n)
{
    if (max_n <= mu_max)
        return;

    int *new_mu = (int*)calloc(max_n + 1, sizeof(int));
    int *lp    = (int*)calloc(max_n + 1, sizeof(int));
    int *primes = (int*)calloc(max_n + 1, sizeof(int));
    idx_t pc = 0;

    new_mu[1] = 1;
    for (idx_t i = 2; i <= max_n; ++i) {
        if (lp[i] == 0) {
            lp[i] = (int)i;
            primes[pc++] = (int)i;
            new_mu[i] = -1;
        }
        for (idx_t j = 0; j < pc && (idx_t)primes[j] <= (idx_t)lp[i] &&
             i * (idx_t)primes[j] <= max_n; ++j) {
            idx_t k = i * (idx_t)primes[j];
            lp[k] = primes[j];
            if (primes[j] == lp[i]) {
                new_mu[k] = 0;
            } else {
                new_mu[k] = -new_mu[i];
            }
        }
    }

    free(mu);
    free(lp);
    free(primes);
    mu = new_mu;
    mu_max = max_n;
}

/* Compute the binomial coefficient C(f+4,5) multiplicatively.  */
static seq_t binom5(idx_t f)
{
    seq_t a = f;          /* f */ 
    seq_t b = f + 1;      /* f+1 */
    seq_t c = f + 2;      /* f+2 */
    seq_t d = f + 3;      /* f+3 */
    seq_t e = f + 4;      /* f+4 */
    seq_t prod = a * b;
    prod *= c;
    prod *= d;
    prod *= e;
    return prod / 120;    /* 5! = 120 */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;        /* the sequence is undefined below the offset 2 */

    compute_mu(n);       /* ensure Möbius values up to n are ready */

    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int mu_d = mu[d];
        if (mu_d == 0)
            continue;    /* skip multiples of a square */

        idx_t cnt;
        if (d == 1)
            cnt = n - 1;          /* numbers 2..n */
        else
            cnt = n / d;          /* multiples of d in [2,n] */

        seq_t term = binom5(cnt); /* C(cnt+4,5) */
        if (mu_d == 1)
            result += term;
        else                    /* mu_d == -1 */
            result -= term;
    }
    return result;
}
