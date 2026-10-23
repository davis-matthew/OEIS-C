/*
 * A164343  A positive integer is included if it is a square that contains the
 *          same number of 0's as 1's when represented in binary.
 * https://oeis.org/A164343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 5137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A164343: a positive integer is a
   square whose binary representation contains the same number of
   0's as 1's.  The code scans successive integers, squares them,
   tests the “balanced” property, and caches results for fast
   random access. */
#include <stdlib.h>
#include <stdbool.h>

/* Test whether x has an equal number of 0 and 1 bits in binary. */
static bool balanced(seq_t x)
{
    seq_t y = x;
    unsigned ones = 0;
    unsigned len = 0;
    while (y) {
        if (y & (seq_t)1) ++ones;
        y >>= 1;
        ++len;
    }
    return len > 0 && (ones << 1) == len;
}

/* Static cache of previously found terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t next_k = 1;        /* next integer to square */

/* Return the n-th term (n ≥ 1).  For n == 0 the sequence is undefined;
   the function returns 0, which is harmless for the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* harmless default for n < 1 */

    while (cache_size < (size_t)n) {
        seq_t sq = (seq_t)next_k * (seq_t)next_k;
        ++next_k;
        if (balanced(sq)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
            }
            cache[cache_size++] = sq;
        }
    }
    return cache[n - 1];
}
