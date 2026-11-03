/*
 * A018402  Divisors of 342.
 * https://oeis.org/A018402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 342 (sorted ascending)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: full, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int compare_seqt(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0 */

    static seq_t cache[12];
    static int initialized = 0;

    if (!initialized)
    {
        int idx = 0;
        for (int e2 = 0; e2 <= 1; ++e2)
        {
            for (int e3 = 0; e3 <= 2; ++e3)
            {
                for (int e19 = 0; e19 <= 1; ++e19)
                {
                    seq_t val = 1;
                    if (e2)
                        val *= (seq_t)2;
                    for (int k = 0; k < e3; ++k)
                        val *= (seq_t)3;
                    if (e19)
                        val *= (seq_t)19;
                    cache[idx++] = val;
                }
            }
        }
        /* idx == 12 */
        mem_qsort(cache, 12, sizeof(seq_t), compare_seqt);
        initialized = 1;
    }

    if ((idx_t)(n - 1) >= 12)
        return 0;                 /* undefined beyond the number of divisors */

    return cache[n - 1];
}
