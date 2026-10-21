/*
 * A088882  Nontrivial palindromes in base 10 (i.e., palindromes that are not
 *          RepDigits such as 3, 111, 22222, or 888888888).
 * https://oeis.org/A088882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088882: Nontrivial palindromes in base‑10 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/reverse_decimal.h"
#include "../utils/digit_len.h"

/* Helper: check if a number consists of a single repeated digit */
static bool is_repdigit(seq_t x) {
    if (x == 0) return true;
    int len = digit_len(x);
    seq_t pow10_len_minus1 = pow10(len - 1);
    seq_t first_digit = x / pow10_len_minus1;
    seq_t tmp = x;
    while (tmp > 0) {
        seq_t d = tmp % 10;
        if (d != first_digit) return false;
        tmp /= 10;
    }
    return true;
}

/* Helper: build palindrome from prefix */
static seq_t build_pal(idx_t prefix, int total_len, bool odd) {
    seq_t p = prefix;
    seq_t rev;
    if (odd) {
        rev = reverse_decimal(p / 10);
    } else {
        rev = reverse_decimal(p);
    }
    seq_t factor = pow10(total_len / 2);
    return p * factor + rev;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; under‑offset values are irrelevant */
    idx_t remaining = n;
    for (int L = 1; ; ++L) {
        int prefix_len = (L + 1) / 2;
        idx_t start = (prefix_len == 1) ? 1 : (idx_t)pow10(prefix_len - 1);
        idx_t end   = (idx_t)pow10(prefix_len) - 1;
        bool odd = (L & 1);
        for (idx_t prefix = start; prefix <= end; ++prefix) {
            seq_t val = build_pal(prefix, L, odd);
            if (is_repdigit(val)) continue;
            if (--remaining == 0) return val;
        }
    }
}
