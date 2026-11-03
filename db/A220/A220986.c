/*
 * A220986  The right Aurifeuillian factor of 10^(20n + 10) + 1.
 * https://oeis.org/A220986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(8n+4) + 10^(7n+4) + 5 * 10^(6n+3) + 2 * 10^(5n+3) + 7
 *            * 10^(4n+2) + 2 * 10^(3n+2) + 5 * 10^(2n+1) + 10^(n+1) + 1
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* compute each power of ten only once */
    idx_t e1 = 8 * n + 4;
    idx_t e2 = 7 * n + 4;
    idx_t e3 = 6 * n + 3;
    idx_t e4 = 5 * n + 3;
    idx_t e5 = 4 * n + 2;
    idx_t e6 = 3 * n + 2;
    idx_t e7 = 2 * n + 1;
    idx_t e8 = n + 1;

    seq_t result = pow10(e1);                 /* 10^(8n+4)          */
    result += pow10(e2);                      /* +10^(7n+4)         */
    result += mul_small(pow10(e3), 5);        /* +5·10^(6n+3)       */
    result += mul_small(pow10(e4), 2);        /* +2·10^(5n+3)       */
    result += mul_small(pow10(e5), 7);        /* +7·10^(4n+2)       */
    result += mul_small(pow10(e6), 2);        /* +2·10^(3n+2)       */
    result += mul_small(pow10(e7), 5);        /* +5·10^(2n+1)       */
    result += pow10(e8);                      /* +10^(n+1)          */
    result += 1;                               /* +1                 */

    return result;
}
