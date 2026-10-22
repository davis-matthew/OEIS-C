/*
 * A004000  RATS: Reverse Add Then Sort the digits applied to previous term,
 *          starting with 1.
 * https://oeis.org/A004000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice, easy
 * Formula:   a(1) = 1, a(n+1) = RATS(a(n)) (reverse digits, add, sort digits
 *            ascending); for n >= 20 use the closed forms a(2k-1) =
 *            (37*10^(k-3)+3332)/3 and a(2k) = (167*10^(k-3)+3331)/3, i.e.
 *            "12" 3...3 "4444" and "55" 6...6 "7777" with k-1 digits in
 *            total. The digits are built one at a time so that no inter...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   sort_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sort_digits.h"

static seq_t reverse_digits(seq_t x) {
    seq_t result = 0;
    while (x > 0) {
        result = result * 10 + (x % 10);
        x /= 10;
    }
    return result;
}

static seq_t rats_step(seq_t x) {
    seq_t sum = x + reverse_digits(x);
    return sort_digits(sum);
}

seq_t a(idx_t n) {
    if (n >= 20) {
        idx_t k = (n % 2 == 1) ? (n + 1) / 2 : n / 2;
        idx_t mid = k - 1 - 6;
        int lead, fill, tail;
        if (n % 2 == 1) { lead = 1; fill = 3; tail = 4; }
        else            { lead = 5; fill = 6; tail = 7; }
        seq_t r = 0;
        if (n % 2 == 1) { r = 1; r = r * 10 + 2; }
        else            { r = 5; r = r * 10 + 5; }
        (void)lead;
        for (idx_t i = 0; i < mid; i++) r = r * 10 + fill;
        for (int i = 0; i < 4; i++) r = r * 10 + tail;
        return r;
    }
    seq_t current = 1;
    for (idx_t i = 2; i <= n; i++) {
        current = rats_step(current);
    }
    return current;
}
