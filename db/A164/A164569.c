/*
 * A164569  Primes p such that 11*p+8 are prime numbers.
 * https://oeis.org/A164569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that 11*p+8 is prime
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    static seq_t next_candidate = 3;          /* first odd prime to test   */
    static idx_t  count        = 0;          /* primes found so far      */

#ifdef __cplusplus
    static std::vector<seq_t> cacheVec;      /* cache of qualifying primes */
#else
    static seq_t *cache   = NULL;            /* cache array              */
    static size_t capacity = 0;              /* allocated size           */
#endif

    while (count < n) {
        seq_t p = next_candidate;
        next_candidate += 2;                 /* skip even numbers */

        if (!is_prime_big(p))
            continue;                       /* compound, skip     */

        seq_t q = (seq_t)11 * p + (seq_t)8;
        if (!is_prime_big(q))
            continue;                       /* condition not met  */

#ifdef __cplusplus
        cacheVec.push_back(p);
#else
        if (count + 1 > capacity) {
            size_t new_cap = capacity ? 2 * capacity : 16;
            while (new_cap <= count)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            capacity = new_cap;
        }
        cache[count] = p;
#endif
        count++;
    }

#ifdef __cplusplus
    return cacheVec[n - 1];
#else
    return cache[n - 1];
#endif
}
