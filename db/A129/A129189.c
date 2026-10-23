/*
 * A129189  n-th prime + n-th composite - n.
 * https://oeis.org/A129189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + c_n – n where p_n is the n‑th prime and c_n is the
 *            n‑th composite (starting with 4)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t i = 2;
    idx_t primecnt = 0;
    idx_t compcnt = 0;
    seq_t nth_prime = 0;
    seq_t nth_comp  = 0;

    while (primecnt < n || compcnt < n)
    {
        if (is_prime(i))
        {
            primecnt++;
            if (primecnt == n)
                nth_prime = (seq_t)i;
        }
        else if (i >= 4)   /* composites start at 4 */
        {
            compcnt++;
            if (compcnt == n)
                nth_comp = (seq_t)i;
        }
        i++;
    }

    return nth_prime + nth_comp - (seq_t)n;
}
