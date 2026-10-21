/*
 * A095559  a(n) = 83 written in base 11 - n.
 * https://oeis.org/A095559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 83 written in base (11 - n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const int VALUE = 83;              /* The number to write in varying bases   */
    int base = 11 - (int)n;           /* Desired base (may be negative or 1)    */

    /* Unary base (base 1): repeat '1' VALUE times */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < VALUE; ++i)
            res = res * (seq_t)10 + 1;
        return res;
    }

    /* Base 0 is not defined in this context – return 0 */
    if (base == 0)
        return 0;

    int base_abs = base > 0 ? base : -base;

    /* For |base| == 1 (except base == 1 handled above), representation
       of non‑zero numbers is impossible.  Return 0. */
    if (base_abs == 1)
        return 0;

    /* Convert VALUE to the chosen base.  Digits are stored least
       significant first. */
    long long val = VALUE;
    int digits[128];
    int dcount = 0;
    while (val != 0) {
        long long r = val % base_abs;
        val = val / base;
        if (r < 0) {                  /* adjust for negative bases   */
            r += base_abs;
            val += 1;
        }
        digits[dcount++] = (int)r;
    }
    if (dcount == 0)                   /* case when VALUE == 0       */
        digits[dcount++] = 0;

    /* Build the final decimal number by concatenating the digits
       (each decimal digit 0–9) or multi‑digit decimal digits
       (only digit 10 can appear when base == 11). */
    seq_t res = 0;
    for (int i = dcount - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10)
            res = res * (seq_t)10 + d;
        else
            res = res * (seq_t)100 + d; /* digit 10 becomes "10"      */
    }

    return res;
}
