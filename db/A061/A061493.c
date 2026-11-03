/*
 * A061493  Roman numerals written using 1 for I, 2 for V, 3 for X, 4 for L, 5
 *          for C, 6 for D, 7 for M.
 * https://oeis.org/A061493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 287, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Validated: full, pass, 3999 terms through n = 3999 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): concatenation of digits 1‑7 representing the Roman numeral
 * of n (I=1,V=2,X=3,L=4,C=5,D=6,M=7).  The function works for all n≥1,
 * including values >3999 by repeating the symbol 'M' for thousands. */
#include <stdint.h>

static inline uint8_t roman_digit(unsigned char c) {
    /* Map Roman characters to digits 1‑7 */
    switch (c) {
        case 'I': return 1;
        case 'V': return 2;
        case 'X': return 3;
        case 'L': return 4;
        case 'C': return 5;
        case 'D': return 6;
        case 'M': return 7;
        default:  return 0;   /* should not happen for valid input */
    }
}

static inline void append_digits(seq_t *val, const char *s)
{
    while (*s) {
        *val = (*val * 10) + roman_digit(*s);
        ++s;
    }
}

static const char * const hundreds[10] = {
    "",     "C",   "CC",   "CCC",   "CD",
    "D",    "DC",  "DCC",  "DCCC",  "CM"
};

static const char * const tens[10] = {
    "",     "X",   "XX",   "XXX",   "XL",
    "L",    "LX",  "LXX",  "LXXX",  "XC"
};

static const char * const ones[10] = {
    "",     "I",   "II",   "III",   "IV",
    "V",    "VI",  "VII",  "VIII",  "IX"
};

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence defined only for n≥1 */

    seq_t result = 0;

    idx_t thousands = n / 1000;
    idx_t h = (n / 100) % 10;
    idx_t t = (n / 10) % 10;
    idx_t o = n % 10;

    /* thousands part: repeat 'M' thousands times */
    for (idx_t i = 0; i < thousands; ++i)
        result = result * 10 + 7;   /* digit for 'M' */

    append_digits(&result, hundreds[h]);
    append_digits(&result, tens[t]);
    append_digits(&result, ones[o]);

    return result;
}
