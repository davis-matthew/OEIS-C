/*
 * A199993  Composite numbers whose multiplicative persistence is 3.
 * https://oeis.org/A199993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth composite number with multiplicative persistence 3
 * by brute‑forcing integers.  The results are cached so that successive
 * calls are fast and independent of call order.  The helper routines
 * digit_product() and is_prime_big() are used from the utility library. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found numbers */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next  = 2;          /* next integer to test */

    /* initial allocation */
    if (cap == 0) {
        cap   = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* target number of terms */
    size_t target = (size_t)n;

    /* generate terms until we have `target` of them */
    while (len < target) {
        seq_t candidate = (seq_t)next;
        next++;

        /* compute multiplicative persistence */
        seq_t prod = candidate;
        int   steps = 0;
        seq_t ten   = (seq_t)10;

        while (prod >= ten && steps <= 3) {
            prod  = digit_product(prod);
            steps++;
        }

        /* keep only composite numbers with persistence exactly 3 */
        if (steps == 3 && !is_prime_big(candidate)) {
            if (len == cap) {
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = candidate;
        }
    }

    return cache[n - 1];
}
