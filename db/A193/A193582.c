/*
 * A193582  Persistence of n for sort-and-subtract (A193581).
 * https://oeis.org/A193582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of sort‑and‑subtract steps to reach 0 * (at each
 *            step n is replaced by n - (ascending order of its digits))
 * Range:     u8 n <= 1418, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t curr = (seq_t)n;
    seq_t steps = 0;
    char buf[128];
    size_t len;
    unsigned int cnt[10];
    char asc[128];

    while (curr != 0)
    {
        /* Convert current number to decimal string to count digits */
        seq_to_str(curr, buf, &len);
        for (int i = 0; i < 10; ++i) cnt[i] = 0;
        for (size_t i = 0; i < len; ++i)
            ++cnt[(unsigned int)(buf[i] - '0')];

        /* Build ascending string representing the smallest permutation */
        size_t idx = 0;
        for (int d = 0; d < 10; ++d)
            for (unsigned int k = 0; k < cnt[d]; ++k)
                asc[idx++] = (char)('0' + d);
        asc[idx] = '\0';

        seq_t asc_num = seq_from_str(asc);
        curr = curr - asc_num;
        ++steps;
    }
    return steps;
}
