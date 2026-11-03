/*
 * A082255  Integer part of (concatenation of (8n-3), (8n-2), (8n-1), (8n),
 *          (8n+1), (8n+2), (8n+3), (8n+4) divided by 8).
 * https://oeis.org/A082255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( concat(8n-3,8n-2,8n-1,8n,8n+1,8n+2,8n+3,8n+4) / 8
 *            )
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_seq_t(int e)
{
    seq_t p = 1;
    for (int i = 0; i < e; ++i)
        p *= (seq_t)10;
    return p;
}

static int digits_idx(int idx)          /* number of decimal digits of a positive int idx */
{
    int d = 0;
    while (idx > 0) {
        ++d;
        idx /= 10;
    }
    return d;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t base8 = (seq_t)8 * (seq_t)n;          /* 8*n in seq_t */
    for (int off = 0; off < 8; ++off) {
        seq_t term = base8 - (seq_t)3 + (seq_t)off; /* 8n-3 + off */

        /* compute number of decimal digits of term (term is positive) */
        idx_t tmp = (idx_t)term;          /* term fits in idx_t for counting digits */
        int d = 0;
        while (tmp > 0) {
            ++d;
            tmp /= 10;
        }

        seq_t mul = pow10_seq_t(d);
        result = result * mul + term;
    }

    return result / (seq_t)8;
}
