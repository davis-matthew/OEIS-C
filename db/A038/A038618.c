/*
 * A038618  Primes not containing the digit '0'.
 * https://oeis.org/A038618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth prime that does not contain digit 0
 * Range:     u8 n <= 50, u16 n <= 3610, u32 n <= 4260, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static bool contains_zero(seq_t x)
{
    seq_t t = x;
    while (t != 0) {
        if ((t % 10) == 0) return true;
        t /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* unused, offset is 1 */

    static idx_t cache_size = 0;      /* number of terms stored */
    static size_t cache_cap  = 0;      /* array capacity */
    static seq_t *cache       = NULL; /* array of terms   */
    static seq_t current      = 2;     /* next integer to test */

    while (cache_size < n) {
        if (!contains_zero(current)) {
            if (is_prime_big(current)) {
                /* grow the cache if necessary */
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = current;
            }
        }
        current = current + (seq_t)1;
    }

    return cache[n - 1];
}
