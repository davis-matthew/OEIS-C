/*
 * A217401  Numbers starting with 8.
 * https://oeis.org/A217401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (71*10^floor(log_10(9*n-8))-8)/9
 * Range:     u8 n <= 1, u16 n <= 236, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   decimal_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/decimal_digits_seq_t.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t x = (seq_t)9 * n_seq - (seq_t)8;          /* x = 9*n - 8 */
    int m = decimal_digits_seq_t(x) - 1;           /* m = floor(log10(x)) */
    seq_t pow10_m = (seq_t)1;
    for (int i = 0; i < m; ++i) {
        pow10_m *= (seq_t)10;                      /* 10^m */
    }
    seq_t temp = (seq_t)71 * pow10_m - (seq_t)8;  /* 71*10^m - 8 */
    seq_t part = temp / (seq_t)9;                 /* (71*10^m - 8)/9 */
    return n_seq + part;                          /* final a(n) */
}
