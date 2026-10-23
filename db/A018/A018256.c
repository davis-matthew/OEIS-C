/*
 * A018256  Divisors of 36.
 * https://oeis.org/A018256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: divisors of 36 are numbers of the form 2^a * 3^b with 0 ≤ a,b ≤ 2, sorted ascending. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are only 9 divisors of 36.  Compute them once. */
    static bool initialized = false;
    static seq_t divisors[9];
    if (!initialized)
    {
        int count = 0;
        /* Enumerate all exponent combinations. */
        for (int a = 0; a <= 2; ++a)          /* exponent of 2 */
        {
            seq_t pow2 = 1;                   /* 2^a */
            for (int i = 0; i < a; ++i)
                pow2 *= 2;

            for (int b = 0; b <= 2; ++b)      /* exponent of 3 */
            {
                seq_t pow3 = 1;               /* 3^b */
                for (int j = 0; j < b; ++j)
                    pow3 *= 3;

                divisors[count++] = pow2 * pow3;
            }
        }
        /* Simple insertion sort to ensure ascending order. */
        for (int i = 1; i < 9; ++i)
        {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key)
            {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        initialized = true;
    }

    /* Convert 1‑based index to 0‑based array index. */
    if (n == 0)
        return 0;
    idx_t idx = n - 1;
    if (idx < 9)
        return divisors[idx];
    /* Out of bounds: sequence is finite. */
    return 0;
}
