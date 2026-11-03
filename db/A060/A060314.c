/*
 * A060314  a(n) = n + next number > n beginning with same digit as n.
 * https://oeis.org/A060314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n + next number > n that begins with the same first digit
 *            as n
 * Range:     u8 n <= 38, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t next_same(idx_t n) {
    seq_t num = (seq_t)n;
    /* Determine 10^(digits-1) */
    seq_t power = 1;
    while (num / power >= 10) {
        power *= 10;
    }
    seq_t d = num / power;               /* first digit */
    seq_t upper = (d + 1) * power - 1;    /* largest number with this first digit */
    if (num < upper) {
        return num + 1;                   /* stays within same block */
    } else {
        return d * power * 10;            /* jump to the next block of same first digit */
    }
}

seq_t a(idx_t n) {
    seq_t nxt = next_same(n);
    return (seq_t)n + nxt;
}
