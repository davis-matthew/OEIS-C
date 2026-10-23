/*
 * A153874  Numbers n = abc...k such that a^2*b^2*c^2*...k^2 - 1 = n.
 * https://oeis.org/A153874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153874:
   a(n) = m^2 - 1 where m = product of the decimal digits of a(n). */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for index 0 */

    seq_t m = 2;                         /* current candidate for m */
    idx_t count = 0;                     /* number of terms found so far */

    for (;; ++m) {
        /* n_val = m^2 - 1 */
        seq_t n_val = m * m - 1;

        /* compute product of decimal digits of n_val */
        seq_t prod = 1;
        seq_t tmp  = n_val;
        while (tmp > 0) {
            seq_t d = tmp % 10;          /* digit (0 … 9) */
            if (d == 0) {                /* any zero digit breaks the product */
                prod = 0;
                break;
            }
            prod *= d;                    /* multiply product by digit */
            tmp /= 10;                    /* shift to next digit */
        }

        if (prod == m) {                  /* n_val satisfies the condition */
            ++count;
            if (count == n) return n_val;
        }
    }
    /* the loop is theoretically infinite; in practice it will terminate for
       the requested n before overflow.  Return 0 as a safe fallback. */
    return 0;
}
