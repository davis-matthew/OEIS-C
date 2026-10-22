/*
 * A001577  An operational recurrence.
 * https://oeis.org/A001577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, easy, nonn
 * Formula:   a(n) = Product_{k=1..n} F(k)^F(n+1-k), F = Fibonacci numbers
 *            (F(1) = F(2) = 1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   a001577_powe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001577_powe.h"

seq_t a(idx_t n) {
    uint64_t *fe = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    idx_t i, k;
    seq_t result = 1;
    seq_t fprev = 0, fcur = 1, ftmp;

    fe[0] = 0;
    fe[1] = 1;
    for (i = 2; i <= n + 1; i++) fe[i] = fe[i - 1] + fe[i - 2];

    for (k = 1; k <= n; k++) {
        /* fcur = F(k) */
        if (fcur != 1) result = result * a001577_powe(fcur, fe[n + 1 - k]);
        ftmp = fcur + fprev;
        fprev = fcur;
        fcur = ftmp;
    }

    free(fe);
    return result;
}
