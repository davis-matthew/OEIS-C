/*
 * A100953  Number of partitions of n into relatively prime parts such that
 *          multiplicities of parts are also relatively prime.
 * https://oeis.org/A100953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} Σ_{k|(n/d)} μ(d) μ(k) p(n/(d*k)) where p(m) is
 *            the ordinary partition function and μ is the Möbius function
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t max_n = 0;
static seq_t *pArr = NULL;   /* partition numbers */
static int *muArr = NULL;    /* Möbius function values */

/* Compute partitions p[0..N] using the additive DP and Möbius up to N */
static void ensure_up_to(idx_t N)
{
    if (N <= max_n) return;

    /* Allocate temporary arrays */
    seq_t *new_p = (seq_t*)malloc((N+1) * sizeof(seq_t));
    int *new_mu = (int*)malloc((N+1) * sizeof(int));
    if (!new_p || !new_mu) return; /* allocation failure, but still safe */

    /* Initialize new arrays */
    for (size_t i = 0; i <= N; ++i) new_p[i] = 0;
    new_p[0] = 1;       /* p(0) = 1 */

    /* Compute partition numbers by additive DP */
    for (size_t i = 1; i <= N; ++i)
        for (size_t j = i; j <= N; ++j)
            new_p[j] += new_p[j-i];

    /* Compute Möbius function using a linear sieve */
    int *spf = (int*)calloc((N+1), sizeof(int));      /* smallest prime factor */
    int *prime_list = (int*)calloc((N+1), sizeof(int));
    size_t prime_cnt = 0;
    new_mu[1] = 1;
    for (int i = 2; i <= (int)N; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = i;
            new_mu[i] = -1;
            prime_list[prime_cnt++] = i;
        }
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= spf[i] &&
                         (long long)i * prime_list[pi] <= (long long)N; ++pi) {
            int v = i * prime_list[pi];
            spf[v] = prime_list[pi];
            if (prime_list[pi] == spf[i]) {
                new_mu[v] = 0;
                break;
            } else {
                new_mu[v] = -new_mu[i];
            }
        }
    }
    free(spf);
    free(prime_list);

    /* Free old arrays */
    free(pArr);
    free(muArr);

    /* Store new arrays */
    pArr = new_p;
    muArr = new_mu;
    max_n = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    ensure_up_to(n);

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* Iterate over all divisors d of n */
    for (size_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int mu_d = muArr[d];
        if (mu_d == 0) continue;
        size_t nd = n / d;

        /* Iterate over all divisors k of nd */
        for (size_t k = 1; k <= nd; ++k) {
            if (nd % k != 0) continue;
            int mu_k = muArr[k];
            if (mu_k == 0) continue;
            size_t idx = nd / k;          /* n/(d*k) */
            seq_t val = pArr[idx];

            if ((mu_d * mu_k) > 0)
                pos_sum += val;
            else
                neg_sum += val;
        }
    }
    /* pos_sum >= neg_sum for all n, so subtraction is safe */
    seq_t result = pos_sum;
    result -= neg_sum;
    return result;
}
