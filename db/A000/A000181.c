/*
 * A000181  a(n) = A058087(n,n-4).
 * https://oeis.org/A000181
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=4..n} (-1)^(j-4) * C(j,4) * r_j * (n-j)!, where
 *            r_j = 2n/(2n-j) * C(2n-j,j) = C(2n-j,j) + C(2n-j-1,j-1) are the
 *            rook numbers of the menage board (permutations of n with exactly
 *            4 hits)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   a181_binom, a181_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a181_binom.h"
#include "../utils/a181_g.h"

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0, f = 1;
    idx_t j;
    if (n < 4) return 0;
    if (n <= 64) {
        for (j = n; ; j--) {
            unsigned long long c4 = (unsigned long long)j * (j - 1) * (j - 2) * (j - 3) / 24;
            seq_t r = a181_binom(2 * n - j, j) + a181_binom(2 * n - j - 1, j - 1);
            seq_t t = (seq_t)c4 * r * f;
            if ((j - 4) % 2 == 0) pos += t; else neg += t;
            if (j == 4) break;
            f *= (seq_t)(n - j + 1);
        }
    } else {
        seq_t B = 1; /* C(2n-j, j), starting with C(n,n) */
        for (j = n; ; j--) {
            unsigned long long c4 = (unsigned long long)j * (j - 1) * (j - 2) * (j - 3) / 24;
            seq_t r = B * (seq_t)(2 * n) / (seq_t)(2 * n - j);
            seq_t t = (seq_t)c4 * r * f;
            if ((j - 4) % 2 == 0) pos += t; else neg += t;
            if (j == 4) break;
            f *= (seq_t)(n - j + 1);
            B = B * (seq_t)(2 * n - j + 1) * (seq_t)j;
            B = B / ((seq_t)(2 * n - 2 * j + 2) * (seq_t)(2 * n - 2 * j + 1));
        }
    }
    return pos - neg;
}
