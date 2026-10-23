/*
 * A018383  Divisors of 304.
 * https://oeis.org/A018383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest positive divisor of 304
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    static seq_t divs[10];
    static size_t init = 0;
    if (!init) {
        size_t pos = 0;
        for (idx_t e2 = 0; e2 <= 1; ++e2) {
            seq_t base = 1;
            if (e2)
                base = 19;
            for (idx_t e1 = 0; e1 <= 4; ++e1) {
                seq_t val = base;
                for (idx_t k = 0; k < e1; ++k)
                    val *= (seq_t)2;
                divs[pos++] = val;
            }
        }
        init = 1;
    }
    if (n <= 10)
        return divs[n-1];
    return 0;
}
