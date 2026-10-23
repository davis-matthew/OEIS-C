/*
 * A116612  Values of n such that p(2n+1) mod 12 = 1, where p(j) denotes the
 *          j-th prime.
 * https://oeis.org/A116612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that the prime number p_{2k+1}
 *            satisfies p_{2k+1} ≡ 1 (mod 12)
 * Range:     u8 n <= 66, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for ease of object handling */
    static std::vector<seq_t> cache;
    static idx_t next_k = 1;

    while (cache.size() < n) {
        for ( ; next_k <= UINT64_MAX; ++next_k) {
            uint32_t p = get_prime(2 * next_k + 1);  /* prime p_{2k+1} */
            if ((p % 12u) == 1u) {
                cache.push_back((seq_t)next_k);
                ++next_k;               /* next search starts after this k */
                break;
            }
        }
    }
    return cache[n - 1];
#else
    /* Pure C implementation using realloc for an array of seq_t */
    static seq_t *cache = NULL;
    static idx_t len   = 0;
    static idx_t next  = 1;

    while (len < n) {
        for ( ; next <= UINT64_MAX; ++next) {
            uint32_t p = get_prime(2 * next + 1);  /* prime p_{2k+1} */
            if ((p % 12u) == 1u) {
                seq_t *tmp = (seq_t *)realloc(cache, (len + 1) * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    len = 0;
                    return 0;
                }
                cache = tmp;
                cache[len++] = (seq_t)next;
                ++next;               /* next search starts after this k */
                break;
            }
        }
    }
    return cache[n - 1];
#endif
}
