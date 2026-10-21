/*
 * A074752  Number of combinatorially inequivalent cyclic subgroups of S_n of
 *          order 6. Number of partitions of n of order 6.
 * https://oeis.org/A074752
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) counts partitions of n whose parts divide 6 and whose *
 *            least common multiple is exactly 6 (OEIS A074752). * Computed by
 *            inclusion–exclusion on the allowed parts {1,2,3,6}
 * Range:     u8 n <= 33, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t count_partitions(const int *parts, int part_count, idx_t n)
{
    if (n == 0) return (seq_t)1;
    size_t len = (size_t)(n + 1);

    seq_t *dp = alloc_seq_array(len);
    if (!dp) return (seq_t)0;           /* allocation failure – fallback */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (int pi = 0; pi < part_count; ++pi) {
        int p = parts[pi];
        for (idx_t i = (idx_t)p; i <= n; ++i)
            dp[i] += dp[i - (idx_t)p];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;          /* offset 5 – undefined below */

    /* inclusion–exclusion on divisors of 6 */
    static const int all_parts[]     = {1, 2, 3, 6};
    static const int no2_parts[]     = {1, 3};
    static const int no3_parts[]     = {1, 2};
    static const int no2no3_parts[]  = {1};

    seq_t cnt_all  = count_partitions(all_parts,     4, n);
    seq_t cnt_no2  = count_partitions(no2_parts,     2, n);
    seq_t cnt_no3  = count_partitions(no3_parts,     2, n);
    seq_t cnt_no23 = count_partitions(no2no3_parts,  1, n);

    /* safely compute cnt_all - cnt_no2 - cnt_no3 + cnt_no23 */
    seq_t sum_no = cnt_no2 + cnt_no3;          /* sum_no >= cnt_no23 */
    seq_t sub_no = sum_no - cnt_no23;          /* sub_no <= cnt_all (inclusion–exclusion) */
    seq_t result = cnt_all - sub_no;           /* final non‑negative value */

    return result;
}
