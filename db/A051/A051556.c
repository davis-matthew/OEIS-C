/*
 * A051556  a(n) = number of 0<=k<=n such that n+k divides binomial(n,k).
 * https://oeis.org/A051556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 651, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the integers k (0 ≤ k ≤ n) for which (n + k) | binomial(n, k).
   Algorithm: for each k factorise n + k using a pre‑computed smallest‑prime‑factor
   sieve (2 · n + 1), compute the exponent of each prime p in C(n, k) by Legendre’s
   formula, and compare with the exponent of p in n + k.  Complexity O(n log n). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_size_t.h"

static uint32_t *spf = NULL;      /* smallest prime factor array */
static size_t   spf_size = 0;     /* current allocated size */

/* build spf array up to 'limit' (inclusive) */
static void init_spf(size_t limit)
{
    if (spf && spf_size >= limit + 1) return;
    size_t new_size = limit + 1;
    uint32_t *new_spf = (uint32_t *)malloc(new_size * sizeof(uint32_t));
    for (size_t i = 0; i < new_size; ++i) new_spf[i] = (uint32_t)i;
    size_t sq = isqrt_size_t(limit);
    for (size_t i = 2; i <= sq; ++i)
        if (new_spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
    free(spf);
    spf     = new_spf;
    spf_size = new_size;
}

/* Legendre exponent of p in m! */
static inline unsigned int v_p_factorial(unsigned int m, unsigned int p)
{
    unsigned int cnt = 0;
    while (m) {
        m /= p;
        cnt += m;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    init_spf((size_t)(2 * n) + 1);
    seq_t ans = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t d = n + k;
        idx_t t = d;
        bool ok = true;
        while (t > 1) {
            uint32_t p = spf[t];
            unsigned int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            unsigned int vp = v_p_factorial((unsigned int)n, p)
                            - v_p_factorial((unsigned int)k, p)
                            - v_p_factorial((unsigned int)(n - k), p);
            if (vp < e) { ok = false; break; }
        }
        if (ok) ++ans;
    }
    return ans;
}
