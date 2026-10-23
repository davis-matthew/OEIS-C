/*
 * A014055  Inverse of 46th cyclotomic polynomial.
 * https://oeis.org/A014055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=∑_{k=1}^{22} (-1)^{k+1} a(n-k) (period 46) */
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t tbl[46];
    static bool init = false;
    if (!init) {
        tbl[0] = 1;
        tbl[1] = 1;
        for (idx_t i = 2; i < 46; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 22 && k <= i; ++k) {
                seq_t sign = (k & 1) ? 1 : -1;
                sum += sign * tbl[i - k];
            }
            tbl[i] = sum;
        }
        init = true;
    }
    idx_t pos = n % 46;
    return tbl[pos];
}
