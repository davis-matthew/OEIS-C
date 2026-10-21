/*
 * A081928  Sum of the n smallest numbers having the sum of their digits equal
 *          to n.
 * https://oeis.org/A081928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the sum of the n smallest numbers with digit sum n. The
 *            routine constructs these n numbers by recursive digit building
 *            and accumulates their sum
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper to generate numbers of a given length */
static void dfs(idx_t pos, idx_t rem, seq_t cur, idx_t len,
                idx_t target, seq_t *sum, idx_t *cnt)
{
    if (*cnt >= target) return;

    if (pos == len) {
        if (rem == 0) {
            *sum += cur;
            ++(*cnt);
        }
        return;
    }

    idx_t remaining = len - pos;
    idx_t start = (pos == 0 ? 1 : 0);

    for (idx_t d = start; d <= 9; ++d) {
        if (rem < d) break;
        idx_t rem_after = rem - d;
        if (rem_after > 9 * (remaining - 1)) continue;
        seq_t next = cur * (seq_t)10 + (seq_t)d;
        dfs(pos + 1, rem_after, next, len, target, sum, cnt);
        if (*cnt >= target) break;
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cnt  = 0;
    idx_t target = n;

    /* minimal number of digits needed to reach sum n */
    idx_t len_min = (n + 8) / 9;   /* ceil(n / 9) */

    for (idx_t len = len_min; ; ++len) {
        dfs(0, n, 0, len, target, &sum, &cnt);
        if (cnt >= target) break;
    }

    return sum;
}
