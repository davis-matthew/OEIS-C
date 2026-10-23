/*
 * A176393  a(n) = A176100(n) + 1 = 2*A141468(n) + 1.
 * https://oeis.org/A176393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A141468(n) + 1, where A141468(n) is the n‑th non‑prime
 *            integer
 *            (0,1,4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30,32,33,34,...)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime_small(idx_t x)
{
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (true)
    {
        bool nonprime;
        if (m == 0 || m == 1)
            nonprime = true;
        else
            nonprime = !is_prime_small(m);

        if (nonprime)
        {
            ++count;
            if (count == n)
                return (seq_t)m * 2 + 1;
        }
        ++m;
    }
}
