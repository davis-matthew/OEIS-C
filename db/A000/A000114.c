/*
 * A000114  Number of cusps of principal congruence subgroup Gamma^{hat}(n).
 * https://oeis.org/A000114
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^2)/2)*Product_{p | n, p prime} (1-1/p^2) =
 *            A007434(n)/2 for n >= 3, a(2) = 3
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)3;
    seq_t r = 1;
    bool halved = false;
    idx_t m = n;
    idx_t p;
    for (p = 2; m > 1; p++) {
        if (p * p > m) p = m;
        if (m % p != 0) continue;
        unsigned e = 0;
        while (m % p == 0) { m /= p; e++; }
        unsigned k = 2 * e - 2; /* number of extra factors of p */
        if (!halved && (p % 2 == 1)) {
            r *= ((seq_t)p * (seq_t)p - 1) / 2;
            halved = true;
        } else if (!halved && p == 2 && e >= 2) {
            r *= 3;          /* (4-1) * 2^(2e-2) / 2 = 3 * 2^(2e-3) */
            k -= 1;
            halved = true;
        } else {
            r *= (seq_t)p * (seq_t)p - 1;
        }
        unsigned i;
        for (i = 0; i < k; i++) r *= (seq_t)p;
    }
    return r;
}
