/*
 * A003234  Numbers k such that A003231(A001950(k)) = A001950(A003231(k)) - 1.
 * https://oeis.org/A003234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k >= 1, keeping k when A003231(A001950(k)) =
 *            A001950(A003231(k)) - 1, where L(m) = floor(m*phi) = floor((m +
 *            isqrt(5*m^2))/2), A001950(m) = L(m) + m, A003231(m) =
 *            floor(m*(5+sqrt(5))/2) = L(m) + 2*m. All arithmetic is exact
 *            integer
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a3234_isqrt(uint64_t x)
{
    if (x < 2) return x;
    uint64_t r = x, y = (x >> 1) + 1;
    while (y < r) {
        r = y;
        y = (r + x / r) >> 1;
    }
    return r;
}

static uint64_t a3234_L(uint64_t m)
{
    return (m + a3234_isqrt(5 * m * m)) / 2;
}

static uint64_t a3234_B(uint64_t m)      /* A003231 */
{
    return a3234_L(m) + 2 * m;
}

static uint64_t a3234_U(uint64_t m)      /* A001950 */
{
    return a3234_L(m) + m;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k;
    for (k = 1; ; k++) {
        if (a3234_B(a3234_U(k)) + 1 == a3234_U(a3234_B(k))) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
