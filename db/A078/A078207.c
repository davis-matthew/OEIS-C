/*
 * A078207  a(n) = A078206(n) / A000422(n).
 * https://oeis.org/A078207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078206(n) / A000422(n); computed by a search over the
 *            suffix length m: k = ceil(C*10^m / P) where C = concat(1..n), P
 *            = concat(n..1), and the first m with k*P < (C+1)*10^m gives a(n)
 *            = k. C*10^m is tracked as quotient/remainder mod P to avoid
 *            overflow
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits0.h"

static seq_t A078207_p10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t C = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t d = A078207_p10((idx_t)num_digits0((uint64_t)i));
        C = C * d + (seq_t)i;
    }

    seq_t P = 0;
    for (idx_t i = n; i > 0; --i) {
        seq_t d = A078207_p10((idx_t)num_digits0((uint64_t)i));
        P = P * d + (seq_t)i;
    }

    /* C * 10^m = q * P + r */
    seq_t q = C / P;
    seq_t r = C % P;
    seq_t pow10m = 1;
    bool pow_big = false; /* true once 10^m > P (then any d < P qualifies) */

    for (;;) {
        seq_t d = (r == 0) ? (seq_t)0 : (seq_t)(P - r); /* k*P - C*10^m */
        if (pow_big || d < pow10m) {
            return q + (r == 0 ? (seq_t)0 : (seq_t)1);
        }
        /* advance m: multiply (q, r) by 10 */
        seq_t r2 = 0;
        seq_t qq = 0;
        for (int t = 0; t < 10; ++t) {
            if (r2 >= P - r) {
                r2 = r2 - (P - r);
                qq += 1;
            } else {
                r2 += r;
            }
        }
        q = q * 10 + qq;
        r = r2;
        if (!pow_big) {
            if (pow10m > P / 10) pow_big = true;
            else pow10m *= 10;
        }
    }
}
