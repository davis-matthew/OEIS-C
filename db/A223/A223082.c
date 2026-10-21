/*
 * A223082  Number of n-digit numbers N with distinct digits such that N
 *          divides the reversal of N.
 * https://oeis.org/A223082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of n‑digit numbers with distinct digits such that
 *            N divides the reversal of N. For n > 5 the value is 0
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Reverse the decimal representation of a non‑negative integer. */
static idx_t reverse_decimal(idx_t x)
{
    idx_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Depth‑first search over all n‑digit numbers with distinct digits (no leading zero). */
static void dfs(idx_t pos, idx_t n, uint64_t used, idx_t num, seq_t *cnt)
{
    if (pos == n)
    {
        idx_t rev_val = reverse_decimal(num);
        if (rev_val % num == 0)
            (*cnt)++;
        return;
    }
    for (int d = 0; d <= 9; ++d)
    {
        if (pos == 0 && d == 0)          /* leading zero not allowed */
            continue;
        uint64_t mask = 1ULL << d;
        if (used & mask)                 /* digit already used */
            continue;
        dfs(pos + 1, n, used | mask, num * 10 + d, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 5)            /* known to be zero for n > 5 */
        return 0;
    seq_t cnt = 0;
    dfs(0, n, 0, 0, &cnt);
    return cnt;
}
