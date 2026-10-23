/*
 * A187274  a(n) = n*4^(n/2 - 1)*(9 + (-1)^n).
 * https://oeis.org/A187274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 5*n*4^n, a(2n+1) = (2n+1)*4^(n+1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t res;
    if ((n & 1) == 0) {          /* n is even: n = 2k */
        idx_t k = n / 2;
        seq_t pow4 = seq_pow((seq_t)4, k);   /* 4^k */
        res = (seq_t)(5 * k) * pow4;         /* 5k * 4^k */
    } else {                    /* n is odd: n = 2k+1 */
        idx_t k = n / 2;        /* floor division */
        seq_t pow4 = seq_pow((seq_t)4, k + 1); /* 4^(k+1) */
        res = (seq_t)n * pow4;                /* n * 4^(k+1) */
    }
    return res;
}
