/*
 * A145451  a(n) = (1/2) * ((1 + sqrt(2))^(3^n) + (1 - sqrt(2))^(3^n)).
 * https://oeis.org/A145451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 4*a(n)^3 + 3*a(n), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A145451 using the recurrence */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    seq_t res = seq_one();          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq   = seq_mul(res, res);                /* res^2 */
        seq_t cu   = seq_mul(sq,   res);                /* res^3 */
        seq_t t1   = seq_mul(cu,    (seq_t)4);          /* 4*res^3 */
        seq_t t2   = seq_mul(res,   (seq_t)3);          /* 3*res   */
        res = t1 + t2;                                  /* a(i)    */
    }
    return res;
}
