/*
 * A021153  Decimal expansion of 1/149.
 * https://oeis.org/A021153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021153: digit at position n in the decimal expansion of 1/149 */
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t rem = 1;      /* remainder for long division (starts with 1) */
    seq_t digit = 0;       /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        uint64_t num = rem * 10ULL;   /* shift remainder left by one decimal place */
        digit = (seq_t)(num / 149ULL); /* next digit of 1/149 */
        rem = num % 149ULL;           /* new remainder */
    }
    return digit;
}
