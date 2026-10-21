/*
 * A224511  Roots of squares generated in A221643. That is, S = i^2 XOR
 *          (i+1)^2; increment i; if S is a square then square root of S is
 *          appended to a(n). Initially i=0. XOR is the binary logical
 *          exclusive-or operator.
 * https://oeis.org/A224511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224511: Roots of squares generated in A221643.
    The sequence a(n) is defined by iterating i from 0 upward,
    computing S = i^2 XOR (i+1)^2, and whenever S is a perfect
    square appending sqrt(S) to the sequence.
    This implementation lazily generates terms and caches them
    so that any a(n) can be returned in O(n) time so far.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Local integer square root using binary search.
   Works for any seq_t that supports * , + , >> , <=, and ==. */
static seq_t floor_sqrt(seq_t x) {
    if (x <= (seq_t)1) return x;
    seq_t low = (seq_t)0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) {
            low = mid + 1;
        } else {
            if (mid == (seq_t)0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n) {
    /* Cache and generation state are static to preserve
       results across arbitrary calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t next_i = 0;     /* next i to process */

    /* Initialise cache lazily. */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n. */
    while (cache_size < n) {
        seq_t i    = (seq_t)next_i;
        seq_t ip1  = i + (seq_t)1;
        seq_t S    = (i * i) ^ (ip1 * ip1);
        seq_t root = floor_sqrt(S);
        if (root * root == S) {              /* perfect square */
            if (cache_size >= cache_cap) {    /* grow cache */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;    /* allocation failed */
                cache = tmp;
            }
            cache[cache_size++] = root;
        }
        next_i++;
    }

    return cache[n - 1];
}
