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
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

/* Recursive search used only for n < 6.  The maximum possible length we
   need to examine is 2*n.  digits[] holds the current decimal digits.
   used[] marks which n‑digit windows (interpreted as integers) have
   already been used. */
static void dfs_small(int n, int depth, seq_t cur_val,
                      int *digits, char *used,
                      seq_t *best_num, size_t *best_len, int max_len)
{
    if ((size_t)depth > *best_len || ((size_t)depth == *best_len && cur_val > *best_num))
    {
        *best_len = (size_t)depth;
        *best_num = cur_val;
    }
    if (depth == max_len) return;

    for (int d = 9; d >= 0; --d)
    {
        if (depth == 0 && d == 0) continue;          /* no leading zero */
        int win = 0;
        int valid = 1;

        if (depth + 1 >= n)
        {
            /* compute the last n‑digit window formed by the new digit d */
            for (int i = depth + 1 - n; i < depth + 1; ++i)
            {
                int digit = (i == depth) ? d : digits[i];
                win = win * 10 + digit;
            }
            if (win % 64 != 0 || used[win])
                valid = 0;
        }

        if (valid)
        {
            if (depth + 1 >= n) used[win] = 1;
            digits[depth] = d;
            dfs_small(n, depth + 1, cur_val * 10 + d,
                      digits, used, best_num, best_len, max_len);
            /* backtrack */
            if (depth + 1 >= n) used[win] = 0;
        }
    }
}

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
