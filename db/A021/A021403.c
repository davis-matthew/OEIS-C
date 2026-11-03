/*
 * A021403  Decimal expansion of 1/399.
 * https://oeis.org/A021403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^{n+1}/399) - 10 * floor(10^n/399)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation computes the nth decimal digit of 1/399 (period 18). */

#include <stdint.h>

/* Compute 10^exp mod 399 using binary exponentiation.  exp is idx_t (uint64_t). */
static unsigned long long mod_pow_10(idx_t exp) {
    unsigned long long result = 1ULL;
    unsigned long long base   = 10ULL;
    /* exp is idx_t; cast to unsigned long long for bitwise operations */
    unsigned long long e = (unsigned long long)exp;
    while (e > 0ULL) {
        if (e & 1ULL)
            result = (result * base) % 399ULL;
        base = (base * base) % 399ULL;
        e >>= 1ULL;
    }
    return result;
}

/* Return the nth term of sequence A021403.  The sequence consists of
   the decimal digits of 1/399; each digit is in the range 0–9. */
seq_t a(idx_t n) {
    unsigned long long r   = mod_pow_10(n);      /* r = 10^n mod 399 */
    unsigned long long dig = (r * 10ULL) / 399ULL; /* a(n) = floor(10 * r / 399) */
    return (seq_t)dig;
}
