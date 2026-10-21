/*
 * A095572  a(n) = 90 written in base n.
 * https://oeis.org/A095572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digits of 90 written in base‑n, concatenated as a decimal
 *            string interpreted as an integer (each digit is written as its
 *            decimal value)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t BASE = 90;            /* The number to be written in base n */
    if (n == 0) /* undefined for the OEIS offset, but return 0 for safety */
        return 0;

    /* Special case: base 1 (unary).  The representation is ten‑night
       of the digit 1.  We compute it by repeated multiplication by 10. */
    if (n == 1) {
        seq_t r = 0;
        /* 90 ones: 111…111  (90 digits) */
        for (idx_t i = 0; i < BASE; ++i) {
            r = r * 10 + 1;
        }
        return r;
    }

    /* For n > 1: 10¹…10^k multiplication depends on the size of the
       current base‑n digit.  Digits are in the range [0, n‑1] but n ≤ 90
       (since 90 in base n ≥ n ⇒ n ≤ 90 + 1).  Therefore a digit is
       either <10 (one decimal digit) or 10…90 (two decimal digits). */
    uint64_t val = BASE;          /* The number to convert, here 90 */
    int  digs[32];                /* Enough slots for all possible digits */
    int  len  = 0;

    /* Generate the digits in reverse order (least‑significant first). */
    while (val > 0) {
        digs[len++] = (int)(val % n);
        val /= n;
    }

    /* Build the decimal integer representation, most significant digit first. */
    seq_t r = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digs[i];
        if (d < 10) {
            r = r * 10 + d;      /* One decimal digit */
        } else {
            /* Two decimal digits: multiply by 100 first. */
            r = r * 100 + d;
        }
    }
    return r;
}
