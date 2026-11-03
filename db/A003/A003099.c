/*
 * A003099  a(n) = Sum_{k=0..n} binomial(n,k^2).
 * https://oeis.org/A003099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(sqrt(n))} binomial(n,k^2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 2926 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int a003099_fixed_width(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x <<= 1;
    return x == 0;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t j, k;
    if (a003099_fixed_width()) {
        /* Pascal row with wrapping arithmetic: exact modulo 2^w */
        seq_t *row = mem_new(seq_t, (size_t)(n + 1));
        idx_t i;
        for (i = 0; i <= n; i++) row[i] = 0;
        row[0] = 1;
        for (i = 1; i <= n; i++) {
            for (j = i; j >= 1; j--) row[j] += row[j - 1];
        }
        for (k = 0; k * k <= n; k++) result += row[k * k];
        mem_free(row);
        return result;
    } else {
        seq_t c = 1;
        k = 0;
        for (j = 0; j <= n; j++) {
            if (j > 0) {
                c *= (seq_t)(n - j + 1);
                c /= (seq_t)j;
            }
            if (k * k == j) {
                result += c;
                k++;
            }
        }
        return result;
    }
}
