/*
 * A003875  Degrees of irreducible representations of symmetric group S_11.
 * https://oeis.org/A003875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest (with multiplicity) of 11!/prod(hook
 *            lengths) over all partitions of 11 (hook length formula), sorted
 *            ascending
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   generate_partitions, hook_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/generate_partitions.h"
#include "../utils/hook_product.h"

seq_t a(idx_t n) {
    static seq_t cache[56];
    static int count = 0;
    static bool computed = false;

    if (!computed) {
        int partition[11];
        count = 0;
        generate_partitions(partition, 0, 11, 11, cache, &count);
        /* insertion sort ascending */
        for (int i = 1; i < count; i++) {
            seq_t key = cache[i];
            int j = i - 1;
            while (j >= 0 && cache[j] > key) {
                cache[j + 1] = cache[j];
                j--;
            }
            cache[j + 1] = key;
        }
        computed = true;
    }

    if (n < 1 || n > (idx_t)count) {
        return 0;
    }
    return cache[(int)n - 1];
}
