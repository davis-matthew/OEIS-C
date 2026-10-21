/*
 * A074858  a(n) = a(n-1) + a(n-2) + R(a(n-3)) where a(0) = a(1) = a(2) = 1
 *          and R(n) (A004086) means the reverse of n.
 * https://oeis.org/A074858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n‑1) + a(n‑2) + R(a(n‑3)), with a(0) = a(1) = a(2) = 1.
 *            R(x) is the decimal digit reverse of x
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers for memory allocation and integer types */
#include <stdlib.h>
#include <stdint.h>

/* Reverse the decimal digits of a value of type seq_t. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        seq_t d = x % 10;
        x = x / 10;
        rev = rev * 10 + d;
    }
    return rev;
}

/* Compute the n‑th term of sequence A074858.  */
seq_t a(idx_t n)
{
    /* Static cache to handle non‑monotonic calls. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Handle small indices directly. */
    if (n < 3)
        return (seq_t)1;

    /* Initialize cache with the first three terms if needed. */
    if (cache_len < 3)
    {
        cache = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* Extend the cache up to index n. */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i)
        {
            cache[i] = cache[i - 1] + cache[i - 2] + reverse_digits(cache[i - 3]);
        }
        cache_len = new_len;
    }

    return cache[n];
}
