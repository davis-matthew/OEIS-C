/*
 * A077167  Sum of terms in n-th row of A077164.
 * https://oeis.org/A077167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of A077164; rows are built by search: row r
 *            holds the next r integers coprime to r, each larger than all
 *            earlier terms
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A077167_g(unsigned long long x, unsigned long long y)
{
    while (y) { unsigned long long t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    seq_t last = 0;
    seq_t sum = 0;
    for (idx_t row = 1; row <= n; ++row)
    {
        seq_t m = (last > (seq_t)row) ? last : (seq_t)row;
        idx_t found = 0;
        sum = 0;
        while (found < row)
        {
            ++m;
            unsigned long long r = (unsigned long long)(m % (seq_t)row);
            if (A077167_g((unsigned long long)row, r) == 1)
            {
                ++found;
                sum += m;
            }
        }
        last = m;
    }
    return sum;
}
