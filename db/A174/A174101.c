/*
 * A174101  Numbers k such that 2*k+1 and 6*k+1 are both primes of the form
 *          6*m+1.
 * https://oeis.org/A174101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 366, u32 n <= 230, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer k such that 2*k+1 and 6*k+1 are both primes of the form 6*m+1 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Static cache for generated terms */
static seq_t cache[20000];
static size_t cache_size = 0;
static idx_t current_k = 3;   /* next candidate multiple of 3 */

/* Generate terms until the cache contains at least n terms */
static void generate_up_to(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    while (cache_size < (size_t)n)
    {
        seq_t p1 = (seq_t)current_k * 2 + 1;
        seq_t p2 = (seq_t)current_k * 6 + 1;

        if (is_prime_big(p1) && is_prime_big(p2))
        {
            cache[cache_size++] = current_k;
        }
        current_k += 3;   /* next multiple of 3 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0, return 0 for safety */
        return 0;
    generate_up_to(n);
    return cache[(size_t)n - 1];
}
