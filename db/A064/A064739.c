/*
 * A064739  Primes p such that Fibonacci(p)-1 is divisible by p.
 * https://oeis.org/A064739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p divides Fibonacci(p) – 1.
   For p > 5 this is equivalent to p == 2 or p ≡ ±1 (mod 5).   */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t last_checked = 1;

    while (cache_len < n)
    {
        ++last_checked;
        if (!is_prime_uint64((uint64_t)last_checked))
            continue;
        if (last_checked == 2 ||
            last_checked % 5 == 1 ||
            last_checked % 5 == 4)
        {
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap ? cache_cap * 2 : 16);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
