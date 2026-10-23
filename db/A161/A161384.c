/*
 * A161384  The numbers 0-100 in Spanish lexicographic order.
 * https://oeis.org/A161384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = the nth number among 0..100 sorted by lexicographic order
 *            of their Spanish names
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   cmp_spanish, write_spanish
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/cmp_spanish.h"
#include "../utils/write_spanish.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static array holding 0..100 sorted lexicographically */
    static idx_t order[101];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= 100; ++i) order[i] = i;
        qsort(order, 101, sizeof(idx_t), cmp_spanish);
        initialized = true;
    }

    if (n == 0) return 0;          /* n is 1-based; undefined below 1 */
    if (n-1 < 101) return (seq_t)order[n-1];
    /* For n > 101 the sequence is undefined; return 0 */
    return 0;
}
