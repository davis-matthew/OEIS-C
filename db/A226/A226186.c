/*
 * A226186  Composite numbers with both additive and multiplicative digital
 *          roots prime.
 * https://oeis.org/A226186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_product, digit_sum, grow_seq, is_prime_big, prime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226186: composite numbers whose additive digital root
 * and multiplicative digital root are both prime (2, 3, 5 or 7). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"
#include "../utils/prime_digit.h"

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static seq_t last_checked = 1;  /* last integer examined */

/* Additive digital root. */
static seq_t additive_root(seq_t x)
{
    seq_t r = x;
    while (r >= 10)
        r = digit_sum(r);
    return r;
}

/* Multiplicative digital root. */
static seq_t multiplicative_root(seq_t x)
{
    seq_t r = x;
    while (r >= 10)
        r = digit_product(r);
    return r;
}

seq_t a(idx_t n)
{
    /* Expand cache until it holds n terms. */
    while (cache_size < (size_t)n)
    {
        /* Move to the next integer to test. */
        ++last_checked;

        /* Skip primes and numbers < 2. */
        if (last_checked <= 1 || is_prime_big(last_checked))
            continue;

        /* Compute additive digital root. */
        seq_t ad = additive_root(last_checked);
        if (!prime_digit(ad))
            continue;

        /* Compute multiplicative digital root. */
        seq_t md = multiplicative_root(last_checked);
        if (!prime_digit(md))
            continue;

        /* Store the term. */
        if (cache_size == cache_cap)
            grow_seq(&cache, &cache_cap, cache_size + 1);
        cache[cache_size++] = last_checked;
    }

    return cache[n - 1];
}
