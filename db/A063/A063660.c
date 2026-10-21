/*
 * A063660  Min{ m | m > n and both numbers have at least one common decimal
 *          digit}.
 * https://oeis.org/A063660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the smallest m > n such that n and m have at least one
 *            common decimal digit. The implementation scans forward from n+1
 *            until a match is found
 * Range:     u8 n <= 254, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   commonDigit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/commonDigit.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;
    seq_t m = num + 1;
    while (!commonDigit(num, m)) {
        m++;
    }
    return m;
}
