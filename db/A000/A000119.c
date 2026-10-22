/*
 * A000119  Number of representations of n as a sum of distinct Fibonacci
 *          numbers.
 * https://oeis.org/A000119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = number of subsets of distinct Fibonacci numbers
 *            {1,2,3,5,8,...} summing to n; computed by the recursion
 *            count(m,i) = count(m,i-1) + count(m-F(i),i-1), using the largest
 *            Fibonacci numbers first and cutting off any branch where m
 *            exceeds the sum of the remaining parts
 * Range:     u8 n <= 6765, u16 n <= 6765, u32 n <= 6765, u64 n <= 6765,
 *            u128 n <= 6765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t fib_tab[100];
static uint64_t fib_sum[100]; /* fib_sum[i] = fib_tab[0]+...+fib_tab[i], saturating */

static seq_t count_rep(uint64_t m, int i)
{
    seq_t r = 0;
    if (m == 0) return 1;
    if (i < 0) return 0;
    if (m > fib_sum[i]) return 0;
    if (i == 0 || m <= fib_sum[i - 1])
        r += count_rep(m, i - 1);
    if (fib_tab[i] <= m)
        r += count_rep(m - fib_tab[i], i - 1);
    return r;
}

seq_t a(idx_t n)
{
    int k = 0;
    uint64_t x = 1, y = 2;
    if (n == 0) return 1;
    /* distinct Fibonacci parts: 1, 2, 3, 5, 8, ... */
    while (x <= n) {
        uint64_t t;
        fib_tab[k] = x;
        if (k == 0) fib_sum[k] = x;
        else {
            uint64_t s = fib_sum[k - 1] + x;
            fib_sum[k] = (s < x) ? UINT64_MAX : s;
        }
        k++;
        if (y < x) break; /* overflow guard */
        t = x + y;
        x = y;
        y = (t < x) ? UINT64_MAX : t;
        if (k >= 99) break;
    }
    return count_rep((uint64_t)n, k - 1);
}
