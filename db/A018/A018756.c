/*
 * A018756  Divisors of 980.
 * https://oeis.org/A018756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 980 (ascending order)
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns the n-th divisor of 980 for n ≥ 1.
   For n > 18 the function returns 0, since 980 has exactly 18 divisors. */
seq_t a(idx_t n)
{
    /* sequence starts at offset 1; out of range returns 0 */
    if (n < 1 || n > 18) return (seq_t)0;

    static seq_t divs[18];
    static bool initialized = false;
    if (!initialized)
    {
        /* prime power tables: 2^0..2^2, 5^0..5^1, 7^0..7^2 */
        seq_t p2[3] = { (seq_t)1, (seq_t)2, (seq_t)4 };
        seq_t p5[2] = { (seq_t)1, (seq_t)5 };
        seq_t p7[3] = { (seq_t)1, (seq_t)7, (seq_t)49 };

        idx_t idx = 0;
        for (idx_t i = 0; i < 3; ++i)          /* exponent of 2 */
        {
            for (idx_t j = 0; j < 2; ++j)      /* exponent of 5 */
            {
                for (idx_t k = 0; k < 3; ++k)  /* exponent of 7 */
                {
                    divs[idx++] = p2[i] * p5[j] * p7[k];
                }
            }
        }

        /* insertion sort: no heavy library calls, works for small fixed size */
        for (idx_t i = 1; i < 18; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        initialized = true;
    }

    return divs[n - 1];
}
