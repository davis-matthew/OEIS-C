/*
 * A035306  List prime factors of each number in order (each prime factor is
 *          followed by its power). Start with 1 = {1,1}.
 * https://oeis.org/A035306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1,a(2)=1; for n>2, a(n) is the nth element of the sequence
 *            obtained by listing the prime factorization of each natural
 *            number in ascending order, each prime factor followed by its
 *            exponent. Thus 1 → {1,1}, 2 → {2,1}, 3 → {3,1}, 4 → {2,2}, 5 →
 *            {5,1}, 6 → {2,1,3,1}, …
 * Range:     u8 n <= 972, u16 n <= 4254, u32 n <= 4254, u64 n <= 4254,
 *            u128 n <= 4254, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

/* static storage for the generated sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* capacity of cache */
static size_t cache_size = 0;     /* number of values stored in cache */
static idx_t next_int = 0;        /* last integer processed (0 at start) */

/* append prime and exponent to the cache, growing it if necessary */
static void push_pair(uint64_t prime, uint64_t exp)
{
    if (cache_size + 2 > cache_cap)
        grow_seq(&cache, &cache_cap, cache_size + 2);

    cache[cache_size++] = (seq_t)prime;
    cache[cache_size++] = (seq_t)exp;
}

/* compute the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against n==0 */
        return 0;

    /* extend the cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        ++next_int;                          /* integer to factorize */

        if (next_int == 1) {                 /* special case: 1 → {1,1} */
            push_pair(1, 1);
            continue;
        }

        uint64_t remaining = (uint64_t)next_int;

        /* trial division to factorize remaining */
        for (uint64_t p = 2; p * p <= remaining; ++p) {
            if (remaining % p != 0)
                continue;

            uint64_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            push_pair(p, exp);
        }

        /* if any prime factor > sqrt still remains */
        if (remaining > 1)
            push_pair(remaining, 1);
    }

    return cache[(size_t)n - 1];
}
