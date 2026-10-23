/*
 * A196222  a(n) = A047726(n)^A047726(n), where A047726(n) is the number of
 *          numbers which can be written with n's digits.
 * https://oeis.org/A196222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A047726(n))^(A047726(n))
 * Range:     u8 n <= 101, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct integers obtained by permuting all digits of
   n (using each digit exactly once) where leading zeros are removed.
   Then raises that count to itself. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Count digits of n */
    unsigned cnt[10] = {0};
    idx_t t = n;
    if (t == 0) {
        cnt[0] = 1;
    } else {
        while (t > 0) {
            cnt[(int)(t % 10)]++;
            t /= 10;
        }
    }

    /* Total number of digits */
    unsigned len = 0;
    for (int d = 0; d < 10; ++d) len += cnt[d];

    /* If n contains only zeros, only integer 0 is possible */
    unsigned nonzero = 0;
    for (int d = 1; d < 10; ++d) nonzero += cnt[d];
    if (nonzero == 0) {
        return big_pow((seq_t)1, (seq_t)1);  /* 1^1 = 1 */
    }

    /* Pre‑compute factorials up to len */
    seq_t fact[21];
    fact[0] = (seq_t)1;
    for (unsigned i = 1; i <= len; ++i) fact[i] = fact[i-1] * (seq_t)i;

    unsigned zeros = cnt[0];
    seq_t f = (seq_t)0;   /* count of distinct integers */

    /* Enumerate number of leading zeros m (0 .. zeros) */
    for (unsigned m = 0; m <= zeros; ++m) {
        /* For each choice of the first non‑zero digit after the m zeros */
        for (int d = 1; d < 10; ++d) {
            if (cnt[d] == 0) continue;

            unsigned rest_len = len - (m + 1);  /* remaining digits after fixing first two positions */

            /* Compute denominator: product of factorials of remaining digit counts */
            seq_t denom = fact[zeros - m];
            for (int e = 0; e < 10; ++e) {
                unsigned c = cnt[e];
                if (e == 0) {
                    /* already accounted for in fact[zeros - m] */
                } else if (e == d) {
                    denom *= fact[c - 1];
                } else {
                    denom *= fact[c];
                }
            }

            seq_t term = fact[rest_len] / denom;
            f += term;
        }
    }

    return big_pow(f, f);
}
