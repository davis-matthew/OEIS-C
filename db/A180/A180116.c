/*
 * A180116  A008619(n-1)-fold concatenation of A109613(n).
 * https://oeis.org/A180116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concat_part(A109613(n), ceil(n/2)) A109613(n) = n if n
 *            odd, n+1 if n even a(n) is the decimal string base repeated
 *            ceil(n/2) times. This expands to: result = base *
 *            10^(d*(ceil(n/2)-1)) + ... + base where d = number of decimal
 *            digits of base
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* compute base = (n odd? n : n+1) */
    seq_t base = (seq_t)(n + ((n & 1) == 0 ? 1 : 0));

    /* repeat count = ceil(n/2) = (n+1)/2 for unsigned */
    idx_t repeat = (n + 1) / 2;

    /* number of decimal digits of base */
    int d = digits((idx_t)base);

    /* 10^d as seq_t */
    seq_t pow10d = pow10(d);

    seq_t result = 0;
    for (idx_t i = 0; i < repeat; ++i) {
        result = result * pow10d + base;
    }
    return result;
}
