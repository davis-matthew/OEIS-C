/*
 * A004429  Geometric mean of digits of n (rounded to nearest integer).
 * https://oeis.org/A004429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = round((product of digits of n)^(1/k)), k = number of
 *            digits of n. Computed exactly with integers: a(n) = 0 if any
 *            digit is 0; otherwise a(n) is the unique m >= 1 with (2m-1)^k <=
 *            2^k * P < (2m+1)^k, where P is the digit product. There are no
 *            ties because the right-hand side is even and (2m...
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   a004429_upow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004429_upow.h"

seq_t a(idx_t n) {
    idx_t t = n;
    unsigned __int128 p = 1;
    int k = 0;
    int m;
    unsigned __int128 lhs;

    do {
        int d = (int)(t % 10);
        if (d == 0) return (seq_t)0;
        p *= (unsigned __int128)d;
        k++;
        t /= 10;
    } while (t > 0);

    lhs = a004429_upow(2, k) * p;   /* at most 18^20, which fits in 128 bits */
    m = 1;
    while (m < 9 && a004429_upow((unsigned __int128)(2 * m + 1), k) <= lhs) m++;
    return (seq_t)m;
}
