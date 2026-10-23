/*
 * A143722  Aliquot sequence starting at 48.
 * https://oeis.org/A143722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 48; a(n+1) = A001065(a(n)) (sum of proper divisors). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Compute the sum of proper divisors of n. */
static seq_t sum_divisors(seq_t n)
{
    /* Proper divisors of 0 and 1 are defined to sum to 0. */
    if (n <= (seq_t)1)
        return 0;
    seq_t sum = 1;                         /* 1 is always a proper divisor. */
    seq_t sq  = floor_sqrt(n);
    for (idx_t i = 2; i <= (idx_t)sq; ++i) {
        seq_t si = (seq_t)i;
        if (n % si == 0) {
            seq_t j = n / si;
            sum += si;
            if (j != si)
                sum += j;
        }
    }
    return sum;
}

/* Memoized computation of the aliquot sequence starting at 48. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;    /* capacity of cache array.          */
    static idx_t  size  = 0;    /* number of terms stored (size).    */

    /* Initialisation on the first call. */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)48;      /* a(0) = 48 */
        size = 1;
        cap  = 1;
    }

    /* Return early if the requested term is already computed. */
    if (n < size)
        return cache[n];

    /* Grow the cache to hold terms up to index n. */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        for (/* nothing */; new_cap <= n; new_cap <<= 1)
            ;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute terms from the current size up to n. */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = sum_divisors(cache[i - 1]);
    }
    size = n + 1;

    return cache[n];
}
