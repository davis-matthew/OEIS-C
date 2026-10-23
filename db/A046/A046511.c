/*
 * A046511  Numbers with multiplicative persistence value 2.
 * https://oeis.org/A046511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer with multiplicative persistence
 *            2
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* return the product of the base‑10 digits of a positive integer v */
static seq_t digit_product(seq_t v)
{
    seq_t prod = 1;
    if (v == 0) return 0;          /* product of digits of 0 is 0 */
    while (v != 0) {
        prod *= v % 10;
        v /= 10;
    }
    return prod;
}

/* a(n) returns the n-th number (n >= 1) that has multiplicative persistence 2.
   The sequence starts with 25, 26, 27, 28, 29, 34, ... */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* no defined element for offset < 1 */
    idx_t found = 0;                /* how many persistence‑2 numbers found */
    seq_t val   = 25;               /* first known persistence‑2 number */

    while (found < n) {
        seq_t p = digit_product(val);      /* product of digits of val */
        if (p >= 10) {                    /* must reach at least two digits */
            seq_t dp = digit_product(p);  /* product of digits of that product */
            if (dp <= 9) {                /* next step reaches a single digit */
                ++found;
                if (found == n) return val;
            }
        }
        ++val;                            /* test next integer */
    }
    return 0;   /* never reached */
}
