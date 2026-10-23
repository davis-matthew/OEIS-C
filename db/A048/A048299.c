/*
 * A048299  Let b(n) = A048273(n) = maximal number of prime factors of C(n,k)
 *          for k=0..n; sequence gives smallest value of k achieving b(n).
 * https://oeis.org/A048299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 526, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, build_spf, update_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A048299: the smallest k ≤ n that maximises the number
 * of distinct prime factors of the binomial coefficient C(n,k).
 * The algorithm factors the products n-k and k+1 incrementally
 * and maintains the exponent vector of the current C(n,k).
 *
 * This implementation uses helpers:
 *   - build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
 *   - build_spf(idx_t limit, idx_t **out)
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/update_factor.h"

static seq_t compute_a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe */

    /* Prime list up to n */
    uint64_t *primes = NULL;
    size_t pcount = 0;
    build_primes_upto(n, &primes, &pcount);
    if (!primes) return (seq_t)0;

    /* Smallest prime factor array 0..n */
    idx_t *spf = NULL;
    build_spf(n, &spf);
    if (!spf) { free(primes); return (seq_t)0; }

    /* Map each prime to its index in the primes array */
    idx_t *prime_index = (idx_t*)calloc(n + 1, sizeof(idx_t));
    if (!prime_index) { free(primes); free(spf); return (seq_t)0; }
    for (idx_t i = 0; i <= n; ++i) prime_index[i] = (idx_t)-1;
    for (size_t i = 0; i < pcount; ++i)
        prime_index[(idx_t)primes[i]] = (idx_t)i;

    /* Exponent vector for all primes */
    int *exp = (int*)calloc(pcount, sizeof(int));
    if (!exp) { free(primes); free(spf); free(prime_index); return (seq_t)0; }

    int curr_cnt = 0;          /* number of primes with positive exponent */
    int best_k   = 0;
    int best_max = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (curr_cnt > best_max) { best_max = curr_cnt; best_k = (int)k; }
        if (k == n) break;

        /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        update_factor(n - k, +1, exp, prime_index, spf, pcount, &curr_cnt);
        update_factor(k + 1, -1, exp, prime_index, spf, pcount, &curr_cnt);
    }

    /* Clean up */
    free(primes);
    free(spf);
    free(prime_index);
    free(exp);

    return (seq_t)best_k;
}

seq_t a(idx_t n)
{
    return compute_a(n);
}
