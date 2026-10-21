/*
 * A220571  Composite numbers that are Brazilian.
 * https://oeis.org/A220571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, is_brazilian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220571: Composite numbers that are Brazilian. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/is_brazilian.h"

/* Dynamic array of cached terms.  */
static seq_t *cached   = NULL;
static size_t     cap   = 0;
static size_t     size  = 0;
static uint64_t   last  = 7;   /* last examined integer */

/* Extend the cache to contain at least `n` terms.  */
static void extend(size_t n)
{
    while (size < n) {
        ++last;
        if (last < 8)               /* skip numbers < 8 */
            continue;
        seq_t candidate = (seq_t)last;
        if (isPrimeGeneric(candidate))
            continue;              /* only composites */
        if (!is_brazilian(candidate))
            continue;
        if (size == cap) {
            size_t newcap = cap ? cap << 1 : 128;
            cached = (seq_t *)realloc(cached, newcap * sizeof(seq_t));
            cap = newcap;
        }
        cached[size++] = candidate;
    }
}

/* Public function: a(idx_t n) returns the n-th composite Brazilian number.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but shall not be called with n == 0 */
    extend((size_t)n);
    return cached[(size_t)n - 1];
}
