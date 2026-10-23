/*
 * A116634  Number of partitions of n having exactly one part that is a
 *          multiple of 3.
 * https://oeis.org/A116634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊n/3⌋} p_non3(n-3k) where p_non3(m) is the
 *            number of partitions of m with no part divisible by 3. This
 *            counts partitions of n having exactly one part that is a
 *            multiple of 3
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 160, u64 n <= 545,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* store the partition counts for sums without multiples of 3 */
    static seq_t *p_non3 = NULL;
    static idx_t max_n_cached = 0;

    /* recompute the DP table if a larger n is requested */
    if (n > max_n_cached) {
        idx_t new_n = n;
        seq_t *new_arr = alloc_seq_array((size_t)(new_n + 1));
        if (!new_arr) return (seq_t)0;          /* allocation failure */

        for (idx_t i = 0; i <= new_n; ++i)
            new_arr[i] = (seq_t)0;
        new_arr[0] = (seq_t)1;                  /* one way to partition 0 */

        for (idx_t part = 1; part <= new_n; ++part) {
            if (part % 3 == 0) continue;         /* skip multiples of 3 */
            for (idx_t sum = part; sum <= new_n; ++sum) {
                new_arr[sum] += new_arr[sum - part];
            }
        }

        if (p_non3)
            free_seq_array(p_non3);
        p_non3 = new_arr;
        max_n_cached = new_n;
    }

    /* sum the contributions from the single multiple‑of‑3 part */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t rem = n - 3 * k;
        result += p_non3[rem];
    }
    return result;
}
