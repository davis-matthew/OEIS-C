/*
 * A180105  Counting integers normally (1, 2, 3, 4, 5...), write them as roman
 *          numerals (I, II, III, IV, V...), describe them (one 1, two 1s,
 *          three 1s, one 1 one 5, one 5...), and write them out as numbers
 *          (11, 21, 31, 1115, 15...).
 * https://oeis.org/A180105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = run‑length encode of the Roman representation of n; each
 *            run of identical numerals is replaced by [count][value] in
 *            decimal
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 48, u64 n <= 998,
 *            u128 n <= 3999, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Convert a single Roman numeral character to its decimal value. */
static int roman_value(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default:  return 0;            /* should never occur */
    }
}

seq_t a(idx_t n) {
    /* Build the Roman representation of n. */
    char buf[32];
    idx_t pos = 0;

    /* Thousands (no subtractive form – just repeat 'M'). */
    idx_t thousands = n / 1000;
    for (idx_t i = 0; i < thousands; ++i) {
        buf[pos++] = 'M';
    }

    /* Tables for the remaining digits. */
    static const char *hundreds[10] = {
        "", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"
    };
    static const char *tens[10] = {
        "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"
    };
    static const char *ones[10] = {
        "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"
    };

    idx_t hundreds_digit = (n % 1000) / 100;
    idx_t tens_digit     = (n % 100) / 10;
    idx_t ones_digit     = n % 10;

    /* Append hundreds. */
    const char *s = hundreds[hundreds_digit];
    while (*s) buf[pos++] = *s++;
    /* Append tens. */
    s = tens[tens_digit];
    while (*s) buf[pos++] = *s++;
    /* Append ones. */
    s = ones[ones_digit];
    while (*s) buf[pos++] = *s++;

    buf[pos] = '\0';
    idx_t len = pos;   /* length of Roman string */

    /* Run‑length encode the Roman string. */
    seq_t result = 0;
    int run_count = 1; /* current run length */

    for (idx_t i = 1; i < len; ++i) {
        if (buf[i] == buf[i - 1]) {
            ++run_count;
        } else {
            /* Append previous run. */
            seq_t cnt = (seq_t)run_count;
            int  val = roman_value(buf[i - 1]);
            seq_t v  = (seq_t)val;
            result = result * pow10((idx_t)digit_len(cnt)) + cnt;
            result = result * pow10((idx_t)digit_len(v))  + v;
            run_count = 1;
        }
    }

    /* Append the final run. */
    if (len > 0) {
        seq_t cnt = (seq_t)run_count;
        int  val = roman_value(buf[len - 1]);
        seq_t v  = (seq_t)val;
        result = result * pow10((idx_t)digit_len(cnt)) + cnt;
        result = result * pow10((idx_t)digit_len(v))  + v;
    }

    return result;
}
