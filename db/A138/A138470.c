/*
 * A138470  Number of numbers less than n having a smaller sum of digits.
 * https://oeis.org/A138470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   * a(n) = #{ m | 0 <= m < n, digit_sum(m) < digit_sum(n) }. * The
 *            function is implemented by a digit–DP that counts the * numbers
 *            < n with a smaller digit sum in O(digits * target_sum) * time,
 *            where target_sum is the sum of digits of n
 * Range:     u8 n <= 268, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DIGITS 20           /* 64‑bit numbers have at most 20 decimal digits */

/* ------------------------------------------------------------ */
/* helper to compute sum of decimal digits of an idx_t          */
static int digit_sum(idx_t x)
{
    int s = 0;
    while (x > 0)
    {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* helper to extract decimal digits of an idx_t into an array |
 *  digits[0] will hold the most significant digit.
 */
static void get_digits(idx_t x, int *digits, int *len)
{
    int pos = 0;
    if (x == 0)
    {
        digits[0] = 0;
        *len = 1;
        return;
    }
    while (x > 0)
    {
        digits[pos++] = x % 10;
        x /= 10;
    }
    /* reverse to most‑significant first */
    for (int i = 0, j = pos - 1; i < j; ++i, --j)
    {
        int tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
    }
    *len = pos;
}

/* ------------------------------------------------------------ */
/* Digit DP implementation */
static seq_t memo[MAX_DIGITS][2][577];
static unsigned char vis[MAX_DIGITS][2][577];
static int  cur_digits[MAX_DIGITS];
static int  cur_len;
static int  target_sum;

/* recursive DFS */
static seq_t dfs(int pos, int tight, int sum)
{
    if (sum >= target_sum) return 0;
    if (pos == cur_len) return 1;            /* reached the end, sum < target_sum */

    if (vis[pos][tight][sum]) return memo[pos][tight][sum];
    vis[pos][tight][sum] = 1;

    seq_t res = 0;
    int limit = tight ? cur_digits[pos] : 9;
    for (int d = 0; d <= limit; ++d)
    {
        int ntight = (tight && (d == cur_digits[pos])) ? 1 : 0;
        res += dfs(pos + 1, ntight, sum + d);
    }
    memo[pos][tight][sum] = res;
    return res;
}

/* ------------------------------------------------------------ */
/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset 0: a(0)=0 */

    idx_t limit = n - 1;                    /* we need numbers < n */
    target_sum = digit_sum(n);

    /* prepare digits of limit */
    get_digits(limit, cur_digits, &cur_len);

    /* clear memoisation tables for the needed range */
    for (int i = 0; i < cur_len; ++i)
        for (int t = 0; t < 2; ++t)
            memset(vis[i][t], 0, target_sum * sizeof(unsigned char));

    return dfs(0, 1, 0);
}
