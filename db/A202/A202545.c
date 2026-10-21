/*
 * A202545  Number of n X 4 nonnegative integer arrays each row and column an
 *          ascent sequence (interior element no greater than one plus
 *          up-steps preceding it).
 * https://oeis.org/A202545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence * a(n) = 36*a(n-1) -
 *            492*a(n-2) + 3418*a(n-3) - 13215*a(n-4) * + 28650*a(n-5) -
 *            32500*a(n-6) + 15000*a(n-7) * initial values: * a(1)=1, a(2)=8,
 *            a(3)=86, a(4)=1094, a(5)=15184, * a(6)=219934, a(7)=3249298
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline seq_t mul_small(seq_t x, int coeff)
{
    seq_t res = 0;
    int c = coeff;
    while (c) {
        if (c & 1) res += x;
        x += x;
        c >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;

    if (n <= seq_len && n < 8)
        return seq[(size_t)n];

    /* ensure capacity up to n */
    if (n > seq_len) {
        seq = (seq_t *)realloc(seq, (size_t)(n + 1) * sizeof(seq_t));
        if (seq == NULL) abort();

        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 7) {
                switch ((int)i) {
                    case 1: val = 1; break;
                    case 2: val = 8; break;
                    case 3: val = 86; break;
                    case 4: val = 1094; break;
                    case 5: val = 15184; break;
                    case 6: val = 219934; break;
                    case 7: val = 3249298; break;
                    default: val = 0; /* unreachable */
                }
            } else {
                seq_t pos = mul_small(seq[(size_t)(i - 1)], 36) +
                            mul_small(seq[(size_t)(i - 3)], 3418) +
                            mul_small(seq[(size_t)(i - 5)], 28650) +
                            mul_small(seq[(size_t)(i - 7)], 15000);
                seq_t neg = mul_small(seq[(size_t)(i - 2)], 492) +
                            mul_small(seq[(size_t)(i - 4)], 13215) +
                            mul_small(seq[(size_t)(i - 6)], 32500);
                val = pos - neg;  /* pos ≥ neg for all i ≥ 8 */
            }
            seq[(size_t)i] = val;
        }
        seq_len = n;
    }

    return seq[(size_t)n];
}
