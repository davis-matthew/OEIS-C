/*
 * A075695  a(1)=1, then "jump over two primes": a(n) =
 *          2*nextprime(nextprime(a(n-1))) - a(n-1).
 * https://oeis.org/A075695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(n)=2*nextprime(nextprime(a(n-1)))-a(n-1)
 * Range:     u8 n <= 15, u16 n <= 1727, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < 1)
        return (seq_t)0;    /* undefined for n < 1, but safe return */

    if (n > cache_len) {
        size_t need = (size_t)n;
        if (need > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = need;
        }
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];
                uint64_t p1 = next_prime((uint64_t)prev);
                uint64_t p2 = next_prime(p1);
                cache[i - 1] = ((seq_t)2) * (seq_t)p2 - prev;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
