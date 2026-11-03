/*
 * A061871  |First digit - second digit + third digit - fourth digit ...| = 2.
 * https://oeis.org/A061871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 7164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9857 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternating-digit-sum–2 sequence:
   a(n) is the nth positive integer whose decimal digits d1,d2,…
   satisfy |d1 - d2 + d3 - d4 + …| = 2.
   No formula is supplied; the definition is evaluated by brute‑force search. */

#include <stdint.h>

static int is_alt_sum2(idx_t x) {
    /* Return 1 iff |alternating sum of digits of x| == 2. */
    uint8_t digits[32];
    int len = 0;
    idx_t tmp = x;
    if (tmp == 0) return 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    int sum = 0, sign = 1;
    for (int i = len - 1; i >= 0; --i) {
        sum += sign * digits[i];
        sign = -sign;
    }
    return (sum == 2 || sum == -2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Sequence is undefined for n < 1 */
    idx_t count = 0;
    idx_t num = 1;
    while (1) {
        if (is_alt_sum2(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
        ++num;
    }
}
