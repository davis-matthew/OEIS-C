/*
 * A026318  a(n) = n-th nonnegative integer k satisfying sin(k) < cos(k) <
 *          sin(k+1).
 * https://oeis.org/A026318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A026318: the n‑th nonnegative integer k
   satisfying sin(k) < cos(k) < sin(k+1).
   The function simply scans k = 0,1,2,… until it has found n
   qualifying values.  This is O(n) with a small constant, fast enough
   for all n requested in the tests. */
#include <math.h>

seq_t a(idx_t n)
{
    /* n is indexed from 1 as per OEIS convention. */
    idx_t count = 0;
    idx_t k = 0;

    while (count < n) {
        double dk = (double)k;
        double sk  = sin(dk);
        double ck  = cos(dk);
        double sk1 = sin(dk + 1.0);

        if (sk < ck && ck < sk1) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }

    /* The loop always returns before reaching here; this is only to
       satisfy the compiler that the function returns a value. */
    return 0;
}
