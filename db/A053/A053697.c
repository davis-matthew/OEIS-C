/*
 * A053697  a(n+1)=a(n)+a^(n), where the addition is in base 11 and where
 *          a^(n) is obtained from a(n) by replacing each digit with its
 *          multiplicative inverse modulo 11. Zero digits, if any, are
 *          deleted.
 * https://oeis.org/A053697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1 a(n+1) = a(n) + a^(n) where addition is performed in
 *            base‑11 (digits 0‑9, carry when a sum ≥ 11) and a^(n) is
 *            obtained by replacing each decimal digit d of a(n) with its
 *            multiplicative inverse modulo 11 (0‑digits are deleted; for 1‑9
 *            we have 1→1, 2→6, 3→4, 4→3, 5→9, 6→2, 7→8, 8→7, 9→5)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base‑11 inverse table for decimal digits 0–9
       (index 0 unused, kept as 0 for completeness). */
    static const int inv_tbl[10] = {0, 1, 6, 4, 3, 9, 2, 8, 7, 5};

    /* Maximum number of decimal digits expected for n ≤ 10000
       (~ 4000) – a safe upper bound. */
    static const int MAX_DIGITS = 8192;

    /* Current value of the sequence. */
    seq_t cur = 1;
    if (n <= 1)
        return cur;

    /* Temporary arrays for digits.  They are allocated once per
       invocation and reused for all iterations. */
    int d[MAX_DIGITS];        /* digits of cur  (LSB first) */
    int inv[MAX_DIGITS];      /* digits of a^(cur) (LSB first) */
    int nd[MAX_DIGITS];       /* digits of cur + a^(cur) (LSB first) */

    for (idx_t step = 2; step <= n; ++step) {
        /* 1. Extract decimal digits of cur. */
        int dlen = 0;
        seq_t temp = cur;
        if (temp == 0) {
            d[dlen++] = 0;
        } else {
            while (temp != 0) {
                /* temp % 10 and temp / 10 are well‑defined for seq_t. */
                d[dlen++] = (int)(temp % 10);
                temp /= 10;
            }
        }

        /* 2. Compute a^(cur) by inverting each digit.  Zero digits
           (which cannot appear) would be omitted. */
        int ilen = 0;
        for (int i = 0; i < dlen; ++i) {
            int digit = d[i];
            if (digit == 0)
                continue;                /* delete zero digits, if any */
            inv[ilen++] = inv_tbl[digit];
        }

        /* 3. Add cur and its inverse in base‑11 (carry threshold 11). */
        int carry = 0;
        int nlen = 0;
        int maxlen = (dlen > ilen ? dlen : ilen);
        for (int i = 0; i < maxlen; ++i) {
            int a_d = (i < dlen) ? d[i] : 0;
            int b_d = (i < ilen) ? inv[i] : 0;
            int s   = a_d + b_d + carry;
            nd[nlen++] = s % 11;          /* digit in base‑11 (also 0‑9) */
            carry = s / 11;
        }
        if (carry > 0)                     /* final carry becomes a new digit */
            nd[nlen++] = carry;

        /* 4. Convert the resulting digits back to a decimal integer. */
        seq_t next = 0;
        for (int i = nlen - 1; i >= 0; --i)
            next = next * 10 + nd[i];

        cur = next;
    }

    return cur;
}
