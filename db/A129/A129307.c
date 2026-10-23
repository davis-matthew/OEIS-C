/*
 * A129307  Intersection of A000217 and A005098.
 * https://oeis.org/A129307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027861(n)*(A027861(n)+1)/2 * A027861(n) is the n-th
 *            integer m such that m^2+(m+1)^2 is prime (i.e. 2m(m+1)+1 is
 *            prime). * a(n) is then the triangular number T(m)=m(m+1)/2
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static idx_t *m_cache = NULL;      /* stores the m-values of A027861 */
static size_t m_cache_cap = 0;     /* capacity of m_cache */
static size_t m_cache_len = 0;     /* number of stored elements */

static void
ensure_m_cache(idx_t needed)
{
    if (m_cache_len >= needed)
        return;

    idx_t cur = (m_cache_len == 0) ? 1 : m_cache[m_cache_len - 1] + 1;

    while (m_cache_len < needed) {
        /* test if 2*m*(m+1)+1 is prime */
        seq_t val = ((seq_t)2 * (seq_t)cur * (seq_t)(cur + 1)) + 1;
        if (is_prime_big(val)) {
            ensure_capacity(m_cache_len + 1, &m_cache_cap,
                            (void **)&m_cache, sizeof(idx_t));
            m_cache[m_cache_len++] = cur;
        }
        cur++;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 for safety */

    ensure_m_cache(n);

    seq_t m = (seq_t)m_cache[n - 1];   /* A027861(n) */
    return (m * (m + 1)) / 2;
}
