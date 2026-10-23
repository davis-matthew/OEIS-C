/*
 * A031911  a(n) = prime(7*n - 6).
 * https://oeis.org/A031911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n - 6)
 * Range:     u8 n <= 8, u16 n <= 935, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t k = 7*n - 6;                /* desired prime index */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t filled = 0;          /* number of primes cached */
    static idx_t last_checked = 1;   /* last integer checked for primality */

    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    if (k <= filled)
        return cache[k-1];

    if (k > (idx_t)cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)k)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    idx_t candidate = last_checked + 1;
    if (candidate <= 2) candidate = 2;

    while (filled < k) {
        if (candidate == 2 || (candidate & 1)) {
            if (is_prime(candidate))
                cache[filled++] = candidate;
        }
        candidate++;
    }
    last_checked = candidate - 1;
    return cache[k-1];
}
