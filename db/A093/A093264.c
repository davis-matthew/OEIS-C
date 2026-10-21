/*
 * A093264  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 64.
 * https://oeis.org/A093264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10^n * (10^n - 64) for n ≥ 6. For n < 6 the numbers are
 *            computed by exhaustive search of decimal strings that satisfy
 *            the substring divisibility and uniqueness constraints
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   dfs_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/dfs_small.h"

static seq_t pow10_idx(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Use the closed form for n ≥ 6 */
    if (n >= 6)
    {
        seq_t p = pow10_idx(n);
        return p * (p - (seq_t)64);
    }

    /* Brute‑force search for n < 6 */
    const int max_used_size = 100000;           /* 10^5, enough for n ≤ 5 */
    int digits[12];          /* maximum 2*n for n≤5 → 10 digits, safety margin */
    char used[max_used_size];
    memset(used, 0, sizeof(used));

    seq_t best_num = 0;
    size_t best_len = 0;
    dfs_small((int)n, 0, 0, digits, used,
              &best_num, &best_len, 2 * (int)n);

    return best_num;
}
