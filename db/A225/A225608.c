/*
 * A225608  The largest n-digit number whose first k digits are divisible by
 *          k.
 * https://oeis.org/A225608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A225608:
 * a(n) = largest n‑digit number whose first k digits form a number divisible by k
 * for all 1 ≤ k ≤ n.  The routine performs a depth‑first search to find the
 * lexicographically largest solution. */
#include <stdbool.h>

static const idx_t MAX_N = 25;         /* the sequence stops at 25 digits */

/* Cache for computed values */
static seq_t cache[26] = {0};
static bool cached[26] = {0};

static bool dfs(idx_t cur_len, seq_t cur_val, idx_t target_len, seq_t *res)
{
    if (cur_len == target_len) {        /* reached desired length */
        *res = cur_val;
        return true;
    }

    idx_t next_len = cur_len + 1;
    for (int d = 9; d >= 0; --d) {      /* try larger digits first */
        seq_t cand = cur_val * 10 + (seq_t)d;
        if (cand % (seq_t)next_len == 0) {
            if (dfs(next_len, cand, target_len, res))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    if (n > MAX_N)                      /* no n‑digit number exists for n > 25 */
        return 0;

    if (!cached[n]) {
        seq_t val = 0;
        dfs(0, 0, n, &val);
        cache[n] = val;
        cached[n] = true;
    }
    return cache[n];
}
