/*
 * A000311  Schroeder's fourth problem; also series-reduced rooted trees with
 *          n labeled leaves; also number of total partitions of n.
 * https://oeis.org/A000311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = -(n-1)*a(n-1) + Sum_{k=1..n-1} binomial(n,k)*a(k)*a(n-k)
 *            for n > 1, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/zalloc_s.h"

static seq_t *A_memo = NULL;   /* A_memo[0..cnt-1] computed terms */
static seq_t *row = NULL;      /* row[k] = binomial(rown, k), k = 0..rown */
static idx_t cnt = 0;
static idx_t rown = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t n)
{
    idx_t newcap, i;
    seq_t *na, *nr;
    if (n + 2 <= cap)
        return;
    newcap = cap ? cap : 64;
    while (newcap < n + 2)
        newcap *= 2;
    na = zalloc_s(newcap);
    nr = zalloc_s(newcap);
    for (i = 0; i < cnt; i++)
        na[i] = A_memo[i];
    if (row != NULL)
        for (i = 0; i <= rown; i++)
            nr[i] = row[i];
    A_memo = na;
    row = nr;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cap(n);
    if (cnt == 0) {
        A_memo[0] = (seq_t)0;
        A_memo[1] = (seq_t)1;
        cnt = 2;
        row[0] = (seq_t)1;
        row[1] = (seq_t)1;
        rown = 1;
    }
    while (cnt <= n) {
        idx_t m = cnt, k;
        seq_t s = (seq_t)0;
        /* advance Pascal row to m */
        while (rown < m) {
            rown++;
            row[rown] = (seq_t)0;
            for (k = rown; k >= 1; k--)
                row[k] += row[k - 1];
        }
        for (k = 1; 2 * k < m; k++)
            s += row[k] * A_memo[k] * A_memo[m - k];
        s *= 2;
        if (m % 2 == 0)
            s += row[m / 2] * A_memo[m / 2] * A_memo[m / 2];
        s -= (seq_t)(m - 1) * A_memo[m - 1];
        A_memo[m] = s;
        cnt++;
    }
    return A_memo[n];
}
