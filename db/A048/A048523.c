/*
 * A048523  Primes for which only one iteration of 'Prime plus its digit sum
 *          equals a prime' is possible.
 * https://oeis.org/A048523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime p for which p + digit_sum(p) is prime but
 *            (p + digit_sum(p)) + digit_sum(p + digit_sum(p)) is composite
 * Range:     u8 n <= 12, u16 n <= 639, u32 n <= 664, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_digits.h"

/* Helper: compute decimal digit sum of a seq_t value. */
static seq_t digit_sum_generic(seq_t x)
{
    /* Buffer big enough for typical big integers (hundreds of digits). */
    uint8_t buf[512];
    size_t len = 0;
    get_digits(x, buf, &len);
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)buf[i];
    return sum;
}

/* Helper: test if prime p satisfies the definition. */
static bool qualifies(seq_t p)
{
    seq_t q = p + digit_sum_generic(p);
    if (!is_prime_big(q))
        return false;
    seq_t r = q + digit_sum_generic(q);
    return !is_prime_big(r);
}

/* Return the n‑th element of OEIS sequence A048523. */
seq_t a(idx_t n)
{
    /* Static cache and state. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_cand = 2;   /* next candidate prime to test */

    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1 */

    /* Grow cache until at least n terms are found. */
    while (cache_len < (size_t)n)
    {
        /* Test current candidate. */
        if (is_prime_big((seq_t)last_cand))
        {
            seq_t p = (seq_t)last_cand;
            if (qualifies(p))
            {
                /* Append to cache, expanding if necessary. */
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)        /* allocation failed */
                        return (seq_t)0;
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
            }
        }

        /* Advance to next odd candidate (2 is handled separately). */
        if (last_cand == 2)
            last_cand = 3;
        else
            last_cand += 2;
    }

    return cache[n - 1];
}
