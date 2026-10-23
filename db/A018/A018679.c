/*
 * A018679  Divisors of 846.
 * https://oeis.org/A018679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 846 in increasing order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[12];
    static bool init = false;

    if (!init)
    {
        /* Prime factorization of 846: 2^1 * 3^2 * 47^1 */
        const uint64_t p[3] = { 2, 3, 47 };
        const unsigned e[3] = { 1, 2, 1 };

        size_t idx = 0;
        for (unsigned i = 0; i <= e[0]; ++i)
        {
            seq_t vi = 1;
            for (unsigned ii = 0; ii < i; ++ii) vi *= (seq_t)p[0];

            for (unsigned j = 0; j <= e[1]; ++j)
            {
                seq_t vj = vi;
                for (unsigned jj = 0; jj < j; ++jj) vj *= (seq_t)p[1];

                for (unsigned k = 0; k <= e[2]; ++k)
                {
                    seq_t vk = vj;
                    for (unsigned kk = 0; kk < k; ++kk) vk *= (seq_t)p[2];
                    divisors[idx++] = vk;
                }
            }
        }

        /* Simple bubble sort (size is only 12) */
        for (size_t a = 0; a < 12; ++a)
            for (size_t b = a + 1; b < 12; ++b)
                if (divisors[b] < divisors[a])
                {
                    seq_t tmp = divisors[a];
                    divisors[a] = divisors[b];
                    divisors[b] = tmp;
                }

        init = true;
    }

    if (n == 0) return (seq_t)0; /* offset is 1, but guard for safety */

    if (n <= 12)
        return divisors[(size_t)(n - 1)];
    else
        return (seq_t)0; /* undefined for n > 12 */
}
