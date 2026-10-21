/*
 * A081989  Numbers k such that 1 + product of digits of k is a square greater
 *          than 1.
 * https://oeis.org/A081989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_product, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081989: Numbers k such that 1 + product_of_digits(k) is a square > 1 */

/* The routine generates the sequence on demand, caching all discovered
   terms.  Each call works independently of the call order.  All
   arithmetic is performed with the type seq_t, which may be 8, 16, 32,
   64, 128‑bit, or arbitrary‑precision.  Helper functions from the
   utils/library are used: digit_product for the digit product and
   isqrt_seq for integer square roots. */

/* Standard includes required for the build. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/isqrt_seq.h"

/* Test whether a candidate k satisfies the defining property.  Returns
   true if 1 + product_of_digits(k) is a perfect square greater than 1. */
static inline bool good(seq_t k)
{
    seq_t prod = digit_product(k);
    /* prod == 0 would give 1 + 0 = 1, which is not > 1 */
    if (prod == 0) return false;
    seq_t sum = prod + 1;
    if (sum <= 1) return false;  /* safety check */
    seq_t root = isqrt_seq(sum); /* floor(sqrt(sum)) */
    return (root * root == sum);
}

/* Return the n-th term of the sequence (1‑based index).  The function
   maintains a static cache of all terms discovered so far. */
seq_t a(idx_t n)
{
    /* Cached terms, dynamically resized. */
    static seq_t *cache    = NULL;
    static size_t size    = 0;
    static size_t cap     = 0;
    static idx_t  next    = 1;          /* next integer to test */

    /* Initialise cache on first call. */
    if (!cache)
    {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – caller will ignore */
    }

    /* Grow cache if needed. */
    if (size == cap)
    {
        cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
    }

    /* Generate terms until we have at least n of them. */
    while (size < n)
    {
        seq_t k = (seq_t)next;
        ++next;
        if (good(k))
        {
            cache[size++] = k;
        }
    }

    return cache[n - 1];
}
