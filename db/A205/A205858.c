/*
 * A205858  The least number j such that 6 divides s(k)-s(j), where
 *          k(n)=A205857(n) and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j for the n-th pair (k,j), j<k, ordered by k then j, with
 *            6 | s(k)-s(j) and s(m)=Fibonacci(m+1); computed by direct search
 *            using the period-24 Fibonacci residues mod 6
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A205858_res(idx_t m)
{
    static unsigned char tab[24];
    static int init = 0;
    if (!init)
    {
        unsigned f[26];
        f[0] = 0; f[1] = 1;
        for (int i = 2; i < 26; ++i) f[i] = (f[i-1] + f[i-2]) % 6;
        for (int i = 0; i < 24; ++i) tab[i] = (unsigned char)f[i+1];
        init = 1;
    }
    return tab[m % 24];
}

seq_t a(idx_t n)
{
    idx_t produced = 0;
    for (idx_t k = 2; ; ++k)
    {
        unsigned rk = A205858_res(k);
        for (idx_t j = 1; j < k; ++j)
        {
            if (A205858_res(j) == rk)
            {
                ++produced;
                if (produced == n)
                    return (seq_t)j;
            }
        }
    }
}
