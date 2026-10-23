/*
 * A170585  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 48, a(n) = 46 * (a(n-1)+…+a(n-46)) + 2 for 2 ≤
 *            n < 47, a(n) = 46 * (a(n-1)+…+a(n-46)) – 1081 * a(n-47) for n ≥
 *            47 (derived from the given generating function)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* multiply a seq_t by a small unsigned int using only + and << */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    seq_t res = 0;
    while (m)
    {
        if (m & 1U)
            res += x;
        x <<= 1;
        m >>= 1;
    }
    return res;
}

static seq_t buf[47];
static seq_t sum46;
static idx_t cur_n = 0;
static bool init_done = false;

static void init_sequence(void)
{
    /* a(0)..a(46) */
    for (idx_t n = 0; n < 47; ++n)
    {
        seq_t sumPrev = 0;
        for (idx_t k = 1; k <= n; ++k)
            sumPrev += buf[n - k];
        seq_t p = (n == 0) ? (seq_t)1 : (seq_t)2;
        buf[n] = mul_small(sumPrev, 46) + p;
    }

    /* sum of a(1)..a(46) for the sliding‑window recurrence */
    sum46 = 0;
    for (idx_t i = 1; i < 47; ++i)
        sum46 += buf[i];

    cur_n = 47;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_sequence();

    if (n < 47)
        return buf[(size_t)n];

    while (cur_n <= n)
    {
        seq_t prod1 = mul_small(sum46, 46);
        size_t old_idx = (size_t)((cur_n - 47ULL) % 47ULL);
        seq_t prod2 = mul_small(buf[old_idx], 1081);
        seq_t a_cur = prod1 - prod2;

        size_t store_idx = (size_t)(cur_n % 47ULL);

        /* slide the window */
        seq_t old_val = buf[old_idx];
        buf[store_idx] = a_cur;
        sum46 += a_cur - old_val;

        ++cur_n;
    }

    return buf[(size_t)(n % 47ULL)];
}
