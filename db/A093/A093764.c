/*
 * A093764  Hook products of all partitions of 6.
 * https://oeis.org/A093764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the n-th smallest hook product of all partitions of 6.
 *            The 11 values are obtained by enumerating the 11 partitions of
 *            6, computing each diagram’s hook product, and sorting the
 *            results
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   gen_partitions, hook_product, sort_vals
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gen_partitions.h"
#include "../utils/hook_product.h"
#include "../utils/sort_vals.h"

#define PART_COUNT 11  /* number of partitions of 6 */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t values[PART_COUNT];

    if (!initialized) {
        int cur_parts[10];
        int out_cnt = 0;
        gen_partitions(6, 6, cur_parts, 0, values, &out_cnt);
        sort_vals(values, PART_COUNT);
        initialized = true;
    }

    return (n >= 1 && n <= (idx_t)PART_COUNT) ? values[(int)(n - 1)] : (seq_t)0;
}
