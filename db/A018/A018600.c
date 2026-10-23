/*
 * A018600  Divisors of 704.
 * https://oeis.org/A018600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 704 in ascending order
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    const seq_t N = (seq_t)704;
    seq_t divs[512];
    idx_t cnt = 0;
    for (idx_t i = 1; i * i <= (idx_t)N; ++i)
    {
        if (N % (seq_t)i == 0)
        {
            divs[cnt++] = (seq_t)i;
            seq_t d2 = N / (seq_t)i;
            if (d2 != (seq_t)i)
                divs[cnt++] = d2;
        }
    }
    for (idx_t i = 0; i < cnt; ++i)
    {
        for (idx_t j = i + 1; j < cnt; ++j)
        {
            if (divs[j] < divs[i])
            {
                seq_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }
    if (n <= cnt) return divs[n - 1];
    return 0;
}
