/*
 * A173347  Fibonacci numbers with both a prime number of 0's and a prime
 *          number of 1's in their binary representations.
 * https://oeis.org/A173347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth Fibonacci number whose binary representation
 *            contains a prime number of 0-bits and a prime number of 1-bits
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Search-based implementation) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *seq_cache = NULL;   /* cache of found terms                     */
static size_t cache_size = 0;     /* number of terms stored                   */
static size_t cache_cap  = 0;     /* allocated capacity in seq_cache          */

static idx_t  fib_index = 2;           /* last computed Fibonacci index (F2)      */
static seq_t  f_prev = (seq_t)1;       /* F(n-1)                                   */
static seq_t  f_curr = (seq_t)1;       /* F(n)                                     */

/* Test whether a value has a prime number of 0-bits
   AND a prime number of 1-bits in its binary representation. */
static bool count_bits_good(seq_t val)
{
    idx_t bits = 0;
    idx_t ones = 0;
    seq_t tmp  = val;

    /* count bits and ones */
    while (tmp != 0) {
        ++bits;
        if (tmp & (seq_t)1) {
            ++ones;
        }
        tmp >>= 1;
    }
    idx_t zeros = bits - ones;
    return is_prime(zeros) && is_prime(ones);
}

/* Ensure seq_cache has capacity for at least need items. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Index 1 maps to cache[0] */
    if (n == 0) return (seq_t)0;           /* out of bounds, should not happen */

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* Compute next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++fib_index;

        /* Test condition */
        if (count_bits_good(f_next)) {
            ensure_cache_capacity(cache_size + 1);
            seq_cache[cache_size++] = f_next;
        }
    }
    return seq_cache[n - 1];
}
