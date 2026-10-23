/*
 * A154493  a(n+1)-+a(n)=prime, a(n+1)*a(n)=Average of twin prime pairs,
 *          a(1)=1,a(2)=4.
 * https://oeis.org/A154493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A154493: a(1)=1, a(2)=4.
 * For n ≥ 2, a(n+1) is defined to be the smallest integer > a(n) such that:
 *   1. a(n+1) - a(n)  is prime,
 *   2. a(n+1) + a(n)  is prime,
 *   3. a(n+1) * a(n) is the average of a twin prime pair
 *      (i.e. t-1 and t+1 are both prime, where t = a(n+1)*a(n)).
 *
 * The function a(idx_t n) implements this definition; it memoizes computed
 * values to allow arbitrary-order calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* allocate or extend a static cache of sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;   /* number of terms currently stored (offset 1) */

/* forward declaration of helper that finds the next term */
static seq_t next_term(seq_t prev);

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 0 */
    if (cache_size < (size_t)n) {
        /* extend the cache to size n */
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure: abort program */
            abort();
        }
        seq_cache = new_cache;
        /* compute missing values */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)            /* a(1) */
                seq_cache[i] = (seq_t)1;
            else if (i == 1)        /* a(2) */
                seq_cache[i] = (seq_t)4;
            else
                seq_cache[i] = next_term(seq_cache[i - 1]); /* compute a(i+1) from a(i) */
        }
        cache_size = new_size;
    }
    return seq_cache[n - 1];
}

/* Find the next term given the previous term 'prev' (= a(n)) */
static seq_t next_term(seq_t prev)
{
    seq_t candidate = prev + 1;           /* smallest integer > prev */
    while (true) {
        seq_t diff = candidate - prev;    /* a(n+1) - a(n) */
        if (!is_prime_big(diff))
        {
            ++candidate;
            continue;
        }

        seq_t sum = candidate + prev;    /* a(n+1) + a(n) */
        if (!is_prime_big(sum))
        {
            ++candidate;
            continue;
        }

        seq_t product = prev * candidate;   /* a(n+1) * a(n) */
        if (is_prime_big(product - 1) && is_prime_big(product + 1))
            return candidate;               /* all conditions satisfied */

        ++candidate;
    }
}
