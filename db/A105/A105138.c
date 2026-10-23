/*
 * A105138  Numbers n such that 256n+129 is prime.
 * https://oeis.org/A105138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer m such that 256*m + 129 is prime
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/push_seq.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of candidates found so far */
    static idx_t cache_cap = 0;      /* capacity of cache */
    static idx_t cache_len = 0;      /* number of elements stored */
    static seq_t current_m = 1;      /* next integer to test */

    if (n == 0)
        return 0;  /* undefined for offset 1, but harmless */

    /* enlarge cache until it contains at least n elements */
    while (cache_len < n)
    {
        uint64_t val = (uint64_t)current_m * 256ULL + 129ULL;
        if (is_prime(val))
            push_seq(&cache, &cache_cap, &cache_len, current_m);
        ++current_m;
    }

    return cache[n - 1];
}
