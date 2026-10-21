/*
 * A228640  a(n) = Sum_{d|n} phi(d)*n^(n/d).
 * https://oeis.org/A228640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} φ(d) * n^(n/d) (OEIS A228640)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static void rec(const uint64_t *primes, const uint8_t *exps, size_t fcnt, size_t idx,
                idx_t cur, idx_t orig, seq_t *sum)
{
    if (idx == fcnt) {
        /* Compute φ(cur) */
        idx_t phi = cur;
        idx_t tmp = cur;
        for (idx_t d = 2; d * d <= tmp; ++d) {
            if (tmp % d == 0) {
                while (tmp % d == 0) tmp /= d;
                phi -= phi / d;
            }
        }
        if (tmp > 1) phi -= phi / tmp;

        seq_t pow = pow_uint((uint64_t)orig, orig / cur);
        seq_t term = (seq_t)phi * pow;
        *sum += term;
        return;
    }

    uint64_t mult = 1;
    for (uint8_t e = 0; e <= exps[idx]; ++e) {
        rec(primes, exps, fcnt, idx + 1, cur * mult, orig, sum);
        mult *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t primes[10];
    uint8_t exps[10];
    size_t fcnt = 0;

    idx_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            exps[fcnt] = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exps[fcnt]++;
            }
            primes[fcnt++] = p;
        }
    }
    if (tmp > 1) {
        primes[fcnt] = tmp;
        exps[fcnt++] = 1;
    }

    seq_t sum = 0;
    rec(primes, exps, fcnt, 0, 1, n, &sum);
    return sum;
}
