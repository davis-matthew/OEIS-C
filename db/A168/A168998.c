/*
 * A168998  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 31*(a(n-1)+...+a(n-22)) - 496*a(n-23) where p(n) =
 *            1 if n==0 or n==23, 2 if 1<=n<=22, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t buffer[23] = {0};

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 1; j <= 22 && i >= j; ++j)
        {
            sum += buffer[(size_t)((i - j) % 23)];
        }

        seq_t term;
        if (i == 0 || i == 23)
            term = 1;
        else if (i <= 22)
            term = 2;
        else
            term = 0;

        term += ((seq_t)31) * sum;

        if (i >= 23)
            term -= ((seq_t)496) * buffer[(size_t)((i - 23) % 23)];

        buffer[(size_t)(i % 23)] = term;
    }

    return buffer[(size_t)(n % 23)];
}
