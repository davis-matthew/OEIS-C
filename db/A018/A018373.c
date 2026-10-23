/*
 * A018373  Divisors of 284.
 * https://oeis.org/A018373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 284 in ascending order
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool initialized = false;
    if (!initialized)
    {
        seq_t temp[6];
        idx_t count = 0;
        for (idx_t d = 1; d <= 284; ++d)
        {
            if (284 % d == 0)
            {
                temp[count++] = (seq_t)d;
            }
        }
        for (idx_t i = 0; i < count; ++i)
            divisors[i] = temp[i];
        initialized = true;
    }
    if (n < 1)
        return 0;
    idx_t idx = n - 1;
    if (idx < 6)
        return divisors[idx];
    return 0;
}
