/*
 * A018282  Divisors of 99.
 * https://oeis.org/A018282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 99 in increasing order (0 if n>6)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    const uint64_t M = 99;
    seq_t divs[6];
    int cnt = 0;

    /* find all divisors of 99 */
    for (uint64_t d = 1; d * d <= M; ++d) {
        if (M % d == 0) {
            divs[cnt++] = (seq_t)d;
            if ((uint64_t)d * (uint64_t)d != M)
                divs[cnt++] = (seq_t)(M / d);
        }
    }

    /* simple bubble sort to order them */
    for (int i = 0; i < cnt - 1; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            if (divs[j] < divs[i]) {
                seq_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }

    if ((idx_t)cnt < n) return 0;
    return divs[n - 1];
}
