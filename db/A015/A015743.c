/*
 * A015743  Number of 8's in all the partitions of n into distinct parts.
 * https://oeis.org/A015743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n-8 into distinct parts that do
 *            not contain 8 (equivalently, the number of partitions of n into
 *            distinct parts that contain an 8). The value is obtained by a
 *            standard one‑dimensional DP for distinct parts, excluding part 8
 * Range:     u8 n <= 38, u16 n <= 90, u32 n <= 254, u64 n <= 818,
 *            u128 n <= 2867, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_buf = NULL;   /* buffer holding DP values up to current target */
static idx_t dp_cap = 0;       /* current capacity of dp_buf (in elements) */

seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;               /* no part 8 can occur */

    idx_t target = n - 8;                     /* sum that must be partitioned */

    /* Ensure the DP buffer is large enough */
    if (target + 1 > dp_cap) {
        seq_t *new_buf = (seq_t *)realloc(dp_buf, (target + 1) * sizeof(seq_t));
        if (!new_buf) abort();                /* memory allocation failed */
        dp_buf = new_buf;
        dp_cap = target + 1;
    }

    /* Initialize DP array */
    dp_buf[0] = (seq_t)1;
    for (idx_t i = 1; i <= target; ++i) dp_buf[i] = (seq_t)0;

    /* Standard distinct‑part DP, skipping part 8 */
    for (idx_t part = 1; part <= target; ++part) {
        if (part == 8) continue;              /* do not allow part 8 */
        for (idx_t m = target; m >= part; --m) {
            dp_buf[m] += dp_buf[m - part];
        }
    }

    return dp_buf[target];
}
