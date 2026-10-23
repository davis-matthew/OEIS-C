/*
 * A018319  Divisors of 175.
 * https://oeis.org/A018319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 175.  a(n) = nth divisor of 175, 1‑indexed. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static size_t sz = 0;

    if (sz == 0)
    {
        /* generate divisors of 175 = 5²·7¹ */
        const seq_t base[2] = {5, 7};
        const idx_t   expn[2] = {2, 1};
        size_t count = 0;

        for (idx_t e5 = 0; e5 <= expn[0]; ++e5)
        {
            seq_t pow5 = 1;
            for (idx_t i = 0; i < e5; ++i) pow5 *= base[0];

            for (idx_t e7 = 0; e7 <= expn[1]; ++e7)
            {
                seq_t pow7 = 1;
                for (idx_t i = 0; i < e7; ++i) pow7 *= base[1];

                divisors[count++] = pow5 * pow7;
            }
        }
        /* the nested loops already produce sorted values, but we sort
           to be safe in case of future changes. */
        for (size_t i = 0; i < count; ++i)
            for (size_t j = i + 1; j < count; ++j)
                if (divisors[j] < divisors[i])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        sz = count;
    }

    if (n == 0) return 0;
    if (n <= sz) return divisors[n - 1];
    return 0;   /* undefined for n > 6 */
}
