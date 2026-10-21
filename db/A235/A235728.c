/*
 * A235728  a(n) = |{0 < k < n - 2: 2*m + 1, m*(m+1) - prime(m) and m*(m+1) +
 *          prime(m) are all prime with m = phi(k) + phi(n-k)/2}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A235728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = |{ 0 < k < n-2 : * 2*m+1, m*(m+1)-prime(m) and
 *            m*(m+1)+prime(m) * are all prime, where m = phi(k) + phi(n-k)/2
 *            }| * (phi is Euler's totient, prime(m) is the m-th prime,
 *            1‑indexed)
 * Range:     u8 n <= 19, u16 n <= 375, u32 n <= 375, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */

static uint64_t *phi_cache = NULL;   /* phi_cache[i] = φ(i) for 0 <= i <= size */
static size_t    phi_cache_size = 0; /* last computed index (>=0) */

/* recompute the totient table up to 'limit' inclusive (O(limit log log limit)) */
static void compute_phi_cache(size_t limit)
{
    uint64_t *new_cache = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i)
        new_cache[i] = i;

    for (size_t p = 2; p <= limit; ++p) {
        if (new_cache[p] == p) { /* p is prime */
            for (size_t q = p; q <= limit; q += p) {
                new_cache[q] -= new_cache[q] / p;
            }
        }
    }

    free(phi_cache);
    phi_cache     = new_cache;
    phi_cache_size = limit;
}

/* ensure phi_cache contains values up to 'limit' */
static void ensure_phi_cache(size_t limit)
{
    if (limit > phi_cache_size)
        compute_phi_cache(limit);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 6)                     /* first five terms are zero */
        return 0;

    ensure_phi_cache((size_t)n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n - 3; ++k) {
        uint64_t t1 = phi_cache[(size_t)k];
        uint64_t t2 = phi_cache[(size_t)(n - k)];
        uint64_t m  = t1 + t2 / 2;        /* integer division – phi(n-k) is even */

        if (m == 0)    /* should never happen for n >= 6, but guard against it */
            continue;

        seq_t m_seq = (seq_t)m;
        seq_t p1    = 2 * m_seq + 1;
        seq_t prime_m = (seq_t)get_prime((idx_t)m);
        seq_t prod     = m_seq * (m_seq + 1);
        seq_t p2       = prod - prime_m;
        seq_t p3       = prod + prime_m;

        if (is_prime_big(p1) && is_prime_big(p2) && is_prime_big(p3))
            ++count;
    }
    return count;
}
