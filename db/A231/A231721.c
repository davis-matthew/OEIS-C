/*
 * A231721  Partial sums of phitorials: a(n) =
 *          A001088(1)+A001088(2)+...+A001088(n).
 * https://oeis.org/A231721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} Π_{i=1..k} φ(i) (φ is Euler's totient)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Cached data */
static idx_t max_computed = 0;          /* highest n that has been computed */
static seq_t prod_cache   = (seq_t)1;   /* Π_{i=1..max_computed} φ(i)   */

#ifdef __cplusplus
/* C++ implementation: use std::vector for automatic construction */
static std::vector<seq_t> sum_cache;    /* a(i), indexed by i (sum_cache[0] unused) */
static std::vector<idx_t> phi_cache;    /* φ(i), indexed by i                 */
#else
/* C implementation: use malloc/realloc for raw arrays */
static seq_t *sum_cache = NULL;         /* a(i), indexed by i (sum_cache[0] unused) */
static idx_t *phi_cache = NULL;         /* φ(i), indexed by i                 */
static idx_t sum_cache_cap = 0;         /* current allocated capacity          */
static idx_t phi_cache_cap = 0;         /* current allocated capacity          */
#endif

seq_t a(idx_t n)
{
    /* Return cached value if possible */
    if (n <= max_computed) {
#ifdef __cplusplus
        return sum_cache[n];
#else
        return sum_cache[n];
#endif
    }

    /* Ensure we have space for new indices */
#ifdef __cplusplus
    if (sum_cache.size() <= (size_t)n) sum_cache.resize((size_t)n * 2 + 1);
    if (phi_cache.size() <= (size_t)n) phi_cache.resize((size_t)n * 2 + 1);
#else
    if (n + 1 > sum_cache_cap) {
        sum_cache_cap = (n + 1) * 2;
        sum_cache = (seq_t*)realloc(sum_cache, sum_cache_cap * sizeof(seq_t));
    }
    if (n + 1 > phi_cache_cap) {
        phi_cache_cap = (n + 1) * 2;
        phi_cache = (idx_t*)realloc(phi_cache, phi_cache_cap * sizeof(idx_t));
    }
#endif

    /* Compute φ(i) for 1 ≤ i ≤ n (simple sieve) */
#ifdef __cplusplus
    for (idx_t i = 1; i <= n; ++i) phi_cache[i] = i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_cache[i] == i) {               /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi_cache[j] -= phi_cache[j] / i;
        }
    }
#else
    for (idx_t i = 1; i <= n; ++i) phi_cache[i] = i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_cache[i] == i) {               /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi_cache[j] -= phi_cache[j] / i;
        }
    }
#endif

    /* Extend the cached product and sum up to n */
    seq_t prod = prod_cache;
    seq_t sum  = (max_computed == 0) ? (seq_t)0 : sum_cache[max_computed];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        prod *= (seq_t)phi_cache[i];
        sum  += prod;
#ifdef __cplusplus
        sum_cache[i] = sum;
#else
        sum_cache[i] = sum;
#endif
    }

    /* Update cached state */
    prod_cache   = prod;
    max_computed = n;

    return sum_cache[n];
}
