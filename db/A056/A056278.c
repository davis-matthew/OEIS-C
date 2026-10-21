/*
 * A056278  Number of primitive (aperiodic) word structures of length n which
 *          contain exactly two different symbols.
 * https://oeis.org/A056278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * 2^{(n/d)-1} (for n > 1), a(1) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* Compute the Möbius function μ(n). Return +1, -1 or 0. */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int mu = 1;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
                if (cnt > 1) return 0;    /* square factor → 0 */
            }
            mu = -mu;
        }
    }
    if (t > 1) mu = -mu;          /* remaining prime factor */
    return mu;
}

/* Cache of powers of two: pow2[i] = 2^i (for i >= 0).  */
static seq_t *pow2_cache = NULL;
static idx_t pow2_cache_len = 0;

/* Ensure the cache has entries up to exp (inclusive).  */
static void ensure_pow2_idx(idx_t exp)
{
    if (exp < pow2_cache_len) return;
    idx_t new_len = exp + 1;
    seq_t *new_cache = alloc_seq_array(new_len);
    if (pow2_cache == NULL) {
        new_cache[0] = (seq_t)1;
        for (idx_t i = 1; i < new_len; ++i)
            new_cache[i] = new_cache[i-1] * (seq_t)2;
    } else {
        for (idx_t i = 0; i < pow2_cache_len; ++i)
            new_cache[i] = pow2_cache[i];
        for (idx_t i = pow2_cache_len; i < new_len; ++i)
            new_cache[i] = new_cache[i-1] * (seq_t)2;
        free(pow2_cache);
    }
    pow2_cache = new_cache;
    pow2_cache_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* by definition */

    seq_t result = 0;

    /* iterate over divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        idx_t exp1 = n / d1 - 1;
        ensure_pow2_idx(exp1);
        seq_t term1 = pow2_cache[exp1];
        int mu1 = mobius(d1);
        if (mu1 == 1)  result += term1;
        else if (mu1 == -1)  result -= term1;

        if (d1 != d2) {
            idx_t exp2 = n / d2 - 1;
            ensure_pow2_idx(exp2);
            seq_t term2 = pow2_cache[exp2];
            int mu2 = mobius(d2);
            if (mu2 == 1)  result += term2;
            else if (mu2 == -1)  result -= term2;
        }
    }

    return result;
}
