/*
 * A078586  a(n) is the product of the first n primes of the form 4k+3.
 * https://oeis.org/A078586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the product of the first n primes of the form 4k+3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   grow_seq, next_prime, seq_alloc_new_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/grow_seq.h"
#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static uint64_t last_prime = 0;

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; caller should never ask for n=0 */
        return 0;

    /* Initialise cache if this is the first call. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = seq_alloc_new_array(cache_cap);
        /* first prime of form 4k+3 */
        cache[0] = (seq_t)3;
        cache_len = 1;
        last_prime = 3;
    }

    /* Extend cache up to requested n */
    while (cache_len < n) {
        /* find next prime of the form 4k+3 */
        uint64_t p = last_prime;
        do {
            p = next_prime(p + 1);
        } while (p % 4 != 3);

        /* compute new product */
        if (cache_len >= cache_cap)
            grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);

        cache[cache_len] = seq_mul(cache[cache_len - 1], (seq_t)p);
        cache_len++;

        last_prime = p;
    }

    return cache[n - 1];
}
