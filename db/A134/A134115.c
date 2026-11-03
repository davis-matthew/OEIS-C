/*
 * A134115  Powers of 9 written backwards and sorted.
 * https://oeis.org/A134115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = reverse_decimal(9^(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   ipow, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t exp = n - 1;
    seq_t p = ipow((seq_t)9, exp);
    return reverse_digits(p);
}
