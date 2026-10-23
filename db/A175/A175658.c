/*
 * A175658  Eight bishops and one elephant on a 3 X 3 chessboard: a(n) =
 *          2*Pell(n+1)+2*Pell(n)-2^n, with Pell = A000129.
 * https://oeis.org/A175658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Pell(n+1)+2*Pell(n)-2^n Recurrence: a(0)=1, a(1)=4,
 *            a(2)=10, a(n)=4*a(n-1)-3*a(n-2)-2*a(n-3) for n≥3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    /* Static cache for computed terms */
    static seq_t *values = NULL;
    static idx_t cache_size = 0;
    static idx_t max_index = 0;

    /* First allocation: ensure space for at least three terms */
    if (values == NULL) {
        size_t init_size = 3;
        if ((size_t)n + 1 > init_size)
            init_size = (size_t)n + 1;
        values = seq_alloc_new_array(init_size);
        cache_size = (idx_t)init_size;

        /* Base cases */
        values[0] = (seq_t)1;
        values[1] = (seq_t)4;
        values[2] = (seq_t)10;
        max_index = 2;
    } else if ((idx_t)(n + 1) > cache_size) {
        /* Grow the array to accommodate the requested index */
        size_t new_size = (size_t)n + 1;
        seq_t *new_values = seq_alloc_new_array(new_size);
        for (idx_t i = 0; i < cache_size; ++i)
            new_values[i] = values[i];
        seq_free_array(values);
        values = new_values;
        cache_size = (idx_t)new_size;
    }

    /* Return already computed value if available */
    if (n <= max_index)
        return values[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t t1 = values[i - 1];
        seq_t t2 = values[i - 2];
        seq_t t3 = values[i - 3];
        values[i] = 4 * t1 - 3 * t2 - 2 * t3;
    }
    max_index = n;
    return values[n];
}
