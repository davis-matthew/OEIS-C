/*
 * A018309  Divisors of 156.
 * https://oeis.org/A018309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 156 (in ascending order)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t target = 156;
    seq_t divs[12];
    size_t cnt = 0;

    /* generate all divisors of 156 */
    for (uint64_t i = 1; (seq_t)i * (seq_t)i <= target; ++i)
    {
        if (target % (seq_t)i == 0)
        {
            divs[cnt++] = (seq_t)i;
            if ((seq_t)i != target / (seq_t)i)
                divs[cnt++] = target / (seq_t)i;
        }
    }

    /* insertion sort the result (cnt ≤ 12) */
    for (size_t i = 1; i < cnt; ++i)
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

    if (n >= 1 && n <= cnt)
        return divs[(size_t)(n - 1)];
    else
        return 0;
}
