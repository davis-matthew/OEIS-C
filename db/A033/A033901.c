/*
 * A033901  Sort then Add, a(1)=21.
 * https://oeis.org/A033901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + sorted_asc_digits(a(n-1)), a(1) = 21
 *            sorted_asc_digits(x) is the integer formed by arranging the
 *            decimal digits of x in ascending order (leading zeros are
 *            dropped)
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 61, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the digits of x sorted ascending and return the resulting integer. */
static seq_t sorted_asc_digits(seq_t x) {
    unsigned cnt[10] = {0};
    seq_t temp = x;
    /* Count digits of temp */
    while (temp != (seq_t)0) {
        unsigned d = (unsigned)(temp % (seq_t)10);
        cnt[d]++;
        temp = temp / (seq_t)10;
    }
    /* Build the ascending‑digit number */
    seq_t res = (seq_t)0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned k = 0; k < cnt[d]; ++k) {
            res = res * (seq_t)10 + (seq_t)d;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* a(1) = 21 */
    seq_t current = (seq_t)21;
    if (n <= (idx_t)1) {
        return current;
    }
    for (idx_t i = 2; i <= n; ++i) {
        seq_t add = sorted_asc_digits(current);
        current = current + add;
    }
    return current;
}
