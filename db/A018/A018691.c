/*
 * A018691  Divisors of 867.
 * https://oeis.org/A018691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 867 (3 × 17²)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The divisors of 867 = 3 * 17^2 are 1, 3, 17, 51, 289, 867. */
    static bool init = false;
    static seq_t divs[6];
    static size_t divcnt = 0;

    if (!init)
    {
        init = true;

        /* Generate all divisors from the prime factors 3^1 and 17^2 */
        size_t idx = 0;
        for (size_t e1 = 0; e1 <= 1; ++e1)
        {
            seq_t pow3 = (e1 == 0) ? (seq_t)1 : (seq_t)3;
            for (size_t e2 = 0; e2 <= 2; ++e2)
            {
                seq_t pow17 = (seq_t)1;
                for (size_t k = 0; k < e2; ++k)
                    pow17 = pow17 * (seq_t)17;
                divs[idx++] = pow3 * pow17;
            }
        }
        divcnt = idx;

        /* Simple insertion sort to order the 6 divisors */
        for (size_t i = 1; i < divcnt; ++i)
        {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key)
            {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
    }

    if (n == 0 || (size_t)(n - 1) >= divcnt)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
