/*
 * A030512  Concatenation of first n 2-digit positive integers including
 *          leading zeros.
 * https://oeis.org/A030512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) concatenates the integers 1…n, writing each with at least
 *            two decimal digits (a leading zero for 1–9). Recurrence: a(n+1)
 *            = a(n) * 10^d(n+1) + (n+1) where d(x)=max(2, number of decimal
 *            digits of x)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t t = i;
        unsigned d = 0;
        do { t /= 10; ++d; } while (t);
        if (d < 2) d = 2;
        res = res * pow10(d) + (seq_t)i;
    }
    return res;
}
