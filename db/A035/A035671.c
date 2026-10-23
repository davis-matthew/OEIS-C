/*
 * A035671  Number of partitions of n into parts 7k+5 and 7k+6 with at least
 *          one part of each type.
 * https://oeis.org/A035671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 184, u32 n <= 501, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A035671
   Number of partitions of n into parts 7k+5 and 7k+6
   with at least one part of each type.
   The function uses dynamic programming on the generating functions. */
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n < 1) return result;            /* offset starts at 1 */

    size_t sz = (size_t)n + 1u;           /* arrays 0 … n */

    /* Allocate arrays for the three types of partitions */
    seq_t *dp5    = alloc_seq_array(sz); /* only 7k+5 parts */
    seq_t *dp6    = alloc_seq_array(sz); /* only 7k+6 parts */
    seq_t *dpboth = alloc_seq_array(sz); /* both types allowed */

    if (!dp5 || !dp6 || !dpboth) {
        seq_free_array(dp5);
        seq_free_array(dp6);
        seq_free_array(dpboth);
        return result;
    }

    dp5[0]    = (seq_t)1;   /* empty partition */
    dp6[0]    = (seq_t)1;
    dpboth[0] = (seq_t)1;

    /* Partitions using only parts ≡5 (mod 7) */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 5 + 7 * k;
        if (v > n) break;
        for (idx_t j = v; j <= n; ++j)
            dp5[(size_t)j] += dp5[(size_t)(j - v)];
    }

    /* Partitions using only parts ≡6 (mod 7) */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 6 + 7 * k;
        if (v > n) break;
        for (idx_t j = v; j <= n; ++j)
            dp6[(size_t)j] += dp6[(size_t)(j - v)];
    }

    /* Partitions using both types (all allowed parts) */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 5 + 7 * k;
        if (v > n) break;
        for (idx_t j = v; j <= n; ++j)
            dpboth[(size_t)j] += dpboth[(size_t)(j - v)];
    }
    for (idx_t k = 0; ; ++k) {
        idx_t v = 6 + 7 * k;
        if (v > n) break;
        for (idx_t j = v; j <= n; ++j)
            dpboth[(size_t)j] += dpboth[(size_t)(j - v)];
    }

    /* subtract partitions that use only one type of part */
    result = dpboth[(size_t)n];
    result -= dp5[(size_t)n];
    result -= dp6[(size_t)n];

    /* free allocated memory */
    seq_free_array(dp5);
    seq_free_array(dp6);
    seq_free_array(dpboth);

    return result;
}
