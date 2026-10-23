/*
 * A171126  Numbers k such that A169611(k) = 1.
 * https://oeis.org/A171126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k such that tau(36*k)=2*tau(6*k),
 *            equivalently k has exactly two 3‑smooth divisors. This holds iff
 *            k = 2*m or 3*m with gcd(m,6)=1. The function generates terms on
 *            demand by testing successive integers
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* test whether k has exactly two 3‑smooth divisors */
static inline bool in_sequence(seq_t k)
{
    /* case 1: k = 2*m, m odd, not divisible by 3 */
    if ((k % 4) == 2 && (k % 3) != 0)
        return true;
    /* case 2: k = 3*m, m odd, not divisible by 3 */
    if ((k % 3) == 0 && (k % 2) != 0 && (k % 9) != 0)
        return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* sequence is defined from offset 1 */
        return (seq_t)0;

    /* static state for incremental generation */
    static idx_t produced = 0;   /* number of terms already produced */
    static seq_t next_candidate = 2;   /* next integer to test */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
#endif

    /* generate until we have at least n terms */
    while (produced < n) {
        if (in_sequence(next_candidate)) {
#ifdef __cplusplus
            cache.push_back(next_candidate);
#else
            if (produced == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[produced] = next_candidate;
#endif
            produced++;
        }
        next_candidate++;
    }

    /* return the requested term */
#ifdef __cplusplus
    return cache[(size_t)(n - 1)];
#else
    return cache[(size_t)(n - 1)];
#endif
}
