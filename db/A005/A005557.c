/*
 * A005557  a(n) is the number of Dyck paths of semilength n+6 having its
 *          first peak at height n+1.
 * https://oeis.org/A005557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (n+1)*binomial(n+10,4)/5 = (n+1)(n+7)(n+8)(n+9)(n+10)/120
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 213, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005557_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005557_gcd.h"

seq_t a(idx_t n)
{
    uint64_t f[5];
    uint64_t d = 120;
    seq_t r = 1;
    int i;

    f[0] = (uint64_t)n + 1;
    f[1] = (uint64_t)n + 7;
    f[2] = (uint64_t)n + 8;
    f[3] = (uint64_t)n + 9;
    f[4] = (uint64_t)n + 10;

    for (i = 0; i < 5 && d > 1; i++) {
        uint64_t g = a005557_gcd(f[i], d);
        f[i] /= g;
        d /= g;
    }

    for (i = 0; i < 5; i++)
        r *= (seq_t)f[i];
    return r;
}
