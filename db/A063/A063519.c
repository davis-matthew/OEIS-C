/*
 * A063519  Least composite k such that phi(k+12n) = phi(k)+12n and
 *          sigma(k+12n) = sigma(k) + 12n where phi is the Euler totient
 *          function and sigma is the sum of divisors function.
 * https://oeis.org/A063519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063519
 *  a(n) = the least composite k such that phi(k+12*n)=phi(k)+12*n and
 *  sigma(k+12*n)=sigma(k)+12*n
 *
 *  The implementation searches for the minimal composite k,
 *  evaluating φ and σ via the provided helper functions
 *  foo.h and sigma_of.h (see build dependencies).   */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Cache the computed values of the sequence.
   cache[0] is unused; cache[1] holds a(1), etc. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that the cache has room up to 'needed' (inclusive). */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, (new_cap + 1));
    if (!tmp)                 /* allocation failed – keep old cache */
        return;
    /* initialise new slots to zero (unknown values) */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n ≥ 1 (a(1) is the first term listed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* the definition is for n≥1 */

    /* Convert indices to the proper type for the cache. */
    const size_t idx = (size_t)n;
    ensure_cache(idx);

    /* Return the cached value if it already exists. */
    if (cache[idx] != 0)
        return cache[idx];

    /* Search for the least composite k satisfying the conditions. */
    const seq_t D = (seq_t)(12 * n);      /* 12 * n as seq_t */
    size_t k = 4;                         /* first composite number */
    seq_t result = 0;

    for (;;) {
        /* Skip primes. */
        if (!is_prime((idx_t)k))
        {
            idx_t kd = (idx_t)k + 12 * n; /* k + 12*n */
            seq_t phi_k   = totient((idx_t)k);
            seq_t sigma_k = sigma_of((idx_t)k);
            seq_t phi_kd  = totient(kd);
            seq_t sigma_kd= sigma_of(kd);

            if (phi_kd == phi_k + D && sigma_kd == sigma_k + D) {
                result = (seq_t)k;
                break;
            }
        }
        ++k;
    }

    cache[idx] = result;
    return result;
}
