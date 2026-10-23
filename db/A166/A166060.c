/*
 * A166060  a(n) = 4*3^n - 3*2^n.
 * https://oeis.org/A166060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^n - 3*2^n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   power_of_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=6, a(n)=5*a(n-1)-6*a(n-2) for n>1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/power_of_3.h"

static seq_t power_of_2(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= 2;
    return result;
}

seq_t a(idx_t n) {
    seq_t pow3 = power_of_3(n);
    seq_t pow2 = power_of_2(n);
    return 4 * pow3 - 3 * pow2;
}
