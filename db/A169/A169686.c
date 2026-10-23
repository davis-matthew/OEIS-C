/*
 * A169686  a(n) = sqrt(T(k-1)*T(k)*T(k+1)) as k runs through the terms of
 *          A072221 and T(i)=i*(i+1)/2.
 * https://oeis.org/A169686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(T(k-1)*T(k)*T(k+1)) where k runs through A072221 (k1
 *            = 1, k2 = 4, k_{n+1} = 6*k_n - k_{n-1} + 2). T(i) = i*(i+1)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static caches for already computed values */
static seq_t *a_cache = NULL;
static seq_t *k_cache = NULL;
static seq_t *m_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure all three cache arrays can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 1;
    while (newcap < need) newcap *= 2;

    seq_t *newk = (seq_t *)realloc(k_cache, newcap * sizeof(seq_t));
    seq_t *newm = (seq_t *)realloc(m_cache, newcap * sizeof(seq_t));
    seq_t *newa = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    if (!newk || !newm || !newa) abort();     /* out of memory – terminate */

    k_cache = newk;
    m_cache = newm;
    a_cache = newa;
    cache_cap = newcap;
}

/* Compute terms up to index 'n' (1‑based). */
static void extend_up_to(idx_t n)
{
    size_t target = (size_t)n;
    if (target <= cache_size) return;

    grow_cache(target);

    for (size_t idx = cache_size + 1; idx <= target; ++idx) {
        size_t i = idx - 1;          /* zero‑based array index */

        if (idx == 1) {              /* first term: k = 1, a = 0 */
            k_cache[i] = (seq_t)1;
            m_cache[i] = (seq_t)0;
            a_cache[i] = (seq_t)0;
        } else if (idx == 2) {       /* second term: k = 4 */
            k_cache[i] = (seq_t)4;
            m_cache[i] = (seq_t)3;
            seq_t tri = (k_cache[i] * (k_cache[i] + (seq_t)1)) / (seq_t)2;
            a_cache[i] = tri * m_cache[i];
        } else {                     /* recurrence for k and m */
            seq_t k_prev      = k_cache[i - 1];
            seq_t k_prevprev  = k_cache[i - 2];
            seq_t k_new = (k_prev * (seq_t)6) - k_prevprev + (seq_t)2;
            k_cache[i] = k_new;

            seq_t m_prev      = m_cache[i - 1];
            seq_t m_prevprev  = m_cache[i - 2];
            seq_t m_new = (m_prev * (seq_t)6) - m_prevprev;
            m_cache[i] = m_new;

            seq_t tri = (k_new * (k_new + (seq_t)1)) / (seq_t)2;
            a_cache[i] = tri * m_new;
        }
    }

    cache_size = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside offset – not used */
    extend_up_to(n);
    return a_cache[n - 1];
}
