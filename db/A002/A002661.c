/*
 * A002661  Least integer having Radon random number n.
 * https://oeis.org/A002661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(binomial(n + 1, floor((n + 1) / 2)) / 2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   a002661_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002661_gcd.h"

seq_t a(idx_t n) {
    idx_t m = n + 1;
    idx_t k = m / 2;
    idx_t i;
    seq_t r = 1;
    /* r = C(m-k+i, i) after step i; stop before the final step */
    for (i = 1; i < k; i++) {
        idx_t x = m - k + i;
        idx_t d = i;
        idx_t g = a002661_gcd(x, d);
        x /= g;
        d /= g;
        r = (r / (seq_t)d) * (seq_t)x;
    }
    /* final step: C(m,k) = r * m / k, then take the ceiling of half */
    {
        idx_t x = m;
        idx_t d = k;
        idx_t g = a002661_gcd(x, d);
        seq_t q;
        x /= g;
        d /= g;
        q = r / (seq_t)d;
        if (x % 2 == 0)
            return q * (seq_t)(x / 2);
        if (q % 2 == 0)
            return (q / 2) * (seq_t)x;
        return (q / 2) * (seq_t)x + (seq_t)((x + 1) / 2);
    }
}
