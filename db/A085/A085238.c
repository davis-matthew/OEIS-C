/*
 * A085238  Sort the numbers 2^i and 3^j. Then a(n) is the exponent of the
 *          n-th term.
 * https://oeis.org/A085238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 417, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Merge the sorted sequences of powers of 2 and powers of 3.
   a(n) returns the exponent e such that the n‑th smallest of
   {2^i,3^j : i,j ≥ 0} equals either 2^e or 3^e.  Offset is 1. */
#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1u)            /* a(1) = 0 (the value 1) */
        return 0;

    const double ln2 = log(2.0);
    const double ln3 = log(3.0);

    idx_t i = 1;   /* next exponent for 2^i   */
    idx_t j = 1;   /* next exponent for 3^j   */
    idx_t cnt = 1; /* 2^0 == 3^0 == 1 already counted */

    while (cnt < n) {
        double v2 = i * ln2; /* compare log-values to avoid overflow */
        double v3 = j * ln3;

        if (v2 <= v3) {          /* 2^i is the next term */
            ++cnt;
            if (cnt == n)
                return (seq_t)i;
            ++i;
        } else {                 /* 3^j is the next term */
            ++cnt;
            if (cnt == n)
                return (seq_t)j;
            ++j;
        }
    }

    /* Should never reach here for valid n */
    return 0;
}
