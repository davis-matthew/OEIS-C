/*
 * A088099  Group the natural numbers with at least two members in each group
 *          such that the n-th group sum is a multiple of n: (1, 2), (3, 4,
 *          5), (6, 7, 8), (9, 10, 11, 12, 13, 14, 15), (16, 17, 18, 19), (20,
 *          21, 22, 23, 24, 25, 26, 27, 28), (29, 30, 31, 32, 33, 34), ...
 *          Sequence contains the group sums.
 * https://oeis.org/A088099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of the n-th group, where the numbers are partitioned
 * into consecutive groups of at least two elements each and the sum of the
 * n-th group is a multiple of n.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static state shared between calls: */
    static seq_t *sums = NULL;      /* 1‑based array of group sums */
    static idx_t cache_size = 0;    /* allocated size of sums vector */
    static idx_t computed_max = 0;  /* highest group index computed so far */
    static idx_t next_num = 1;      /* next unused natural number */

    /* enlarge cache as needed */
    if (n >= cache_size) {
        size_t new_capacity = cache_size == 0 ? 16 : cache_size * 2;
        while (new_capacity <= n) new_capacity *= 2;
        seq_t *tmp = (seq_t*)realloc(sums, new_capacity * sizeof(seq_t));
        if (!tmp) return 0; /* fallback – OOM */
        sums = tmp;
        cache_size = new_capacity;
    }

    /* compute missing groups up to n */
    for (idx_t grp = computed_max + 1; grp <= n; ++grp) {
        seq_t sum = 0;
        idx_t cnt  = 0;
        while (1) {
            sum += (seq_t)next_num;
            ++cnt;
            ++next_num;
            if (cnt >= 2 && sum % (seq_t)grp == 0)
                break;
        }
        sums[grp] = sum;
        computed_max = grp;
    }

    return sums[n];
}
