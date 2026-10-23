/*
 * A018417  Divisors of 369.
 * https://oeis.org/A018417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 369 – a(n) is the nth divisor of 369 in ascending order. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the known divisors. */
    if (n < 1 || n > 6)
        return (seq_t)0;

    /* Cache the six divisors after the first call. */
    static seq_t cache[6] = {0};
    static int cached = 0;

    if (!cached) {
        uint64_t divs[6];
        int idx = 0;
        for (uint64_t e1 = 0; e1 <= 2; ++e1) {          /* 3^e1, e1=0..2 */
            uint64_t p3 = 1;
            for (uint64_t i = 0; i < e1; ++i)
                p3 *= 3;
            for (uint64_t e2 = 0; e2 <= 1; ++e2) {      /* 41^e2, e2=0..1 */
                uint64_t prod = p3;
                if (e2)
                    prod *= 41;
                divs[idx++] = prod;
            }
        }
        /* Sort the divisors (simple bubble sort as there are only six). */
        for (int i = 0; i < 6; ++i)
            for (int j = i + 1; j < 6; ++j)
                if (divs[j] < divs[i]) {
                    uint64_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
        for (int i = 0; i < 6; ++i)
            cache[i] = (seq_t)divs[i];
        cached = 1;
    }

    return cache[n - 1];
}
