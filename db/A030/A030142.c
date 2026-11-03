/*
 * A030142  Odd numbers in which parity of digits alternates.
 * https://oeis.org/A030142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth odd integer whose digits alternate parity (i.e., no
 *            two consecutive digits have the same parity, and the last digit
 *            is odd)
 * Range:     u8 n <= 50, u16 n <= 2525, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9541 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool alternating_parity(seq_t x) {
    int prev_parity = -1;                 /* parity of the digit just processed (LSB side) */
    while (x > 0) {
        int digit = (int)(x % 10);        /* current least‑significant digit */
        int parity = digit & 1;           /* 0 = even, 1 = odd */

        /* For the first digit (prev_parity == -1) just record parity */
        if (prev_parity != -1 && parity == prev_parity)
            return false;                /* two neighboring digits with same parity */

        prev_parity = parity;
        x = x / 10;                       /* drop processed digit */
    }
    return true;
}

/* Return the n-th odd integer with alternating parity digits.
   n >= 1.  Uses only integer arithmetic allowed in the spec. */
seq_t a(idx_t n) {
    idx_t count = 0;            /* number of valid terms found so far */
    seq_t cand = 1;             /* current odd candidate */

    while (1) {
        if (alternating_parity(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand += 2;               /* skip to next odd number */
    }
}
