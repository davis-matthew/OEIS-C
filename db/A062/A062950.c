/*
 * A062950  C(H(n)), where C(n) is Chowla's function (A048050) and H(n) is the
 *          half-totient function (A023022).
 * https://oeis.org/A062950
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = C(H(n)), C(m) = sigma(m) - m - 1 (A048050), H(n) =
 *            phi(n)/2 (A023022); C(1) = -1
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Validated: partial, pass, 100 terms through n = 102 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A062950_phi(idx_t n)
{
    idx_t result = n;
    idx_t tmp = n;
    for (idx_t i = 2; i <= tmp / i; i++) {
        if (tmp % i == 0) {
            while (tmp % i == 0)
                tmp /= i;
            result -= result / i;
        }
    }
    if (tmp > 1)
        result -= result / tmp;
    return result;
}

static seq_t A062950_chowla(idx_t m)
{
    if (m <= 1)
        return -1;
    seq_t sum = 0;
    for (idx_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t h = A062950_phi(n) / 2;
    return A062950_chowla(h);
}
