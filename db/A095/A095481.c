/*
 * A095481  a(n) = 44 written in base 14 - n.
 * https://oeis.org/A095481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 44 represented in base (14 - n), interpreted as a decimal
 *            number
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: full, pass, 14 terms through n = 13 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n = 0 … 13 */
    if (n > 13) {
        return 0;          /* undefined for larger n */
    }

    int base = 14 - (int)n;  /* safe cast because n ≤ 13 */

    /* Buffer for the decimal string of the result.
       The longest possible string is 44 '1's (for n = 13). */
    char buf[50];
    size_t len = 0;

    if (base == 1) {
        /* Unary representation: 44 ones */
        for (size_t i = 0; i < 44; ++i) {
            buf[len++] = '1';
        }
    } else {
        int value = 44;          /* 44 in decimal */
        char rev[10];            /* digits in reverse order */
        size_t revlen = 0;

        while (value > 0) {
            int d = value % base;
            rev[revlen++] = '0' + d;
            value /= base;
        }

        /* Place digits in correct order */
        for (size_t i = 0; i < revlen; ++i) {
            buf[len++] = rev[revlen - 1 - i];
        }
    }

    buf[len] = '\0';
    return seq_from_str(buf);
}
