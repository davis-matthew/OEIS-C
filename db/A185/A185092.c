/*
 * A185092  Divisors of 12.
 * https://oeis.org/A185092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth smallest divisor of 12 (0 if n exceeds number of
 *            divisors)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is finite; it consists of the divisors of 12:
       1, 2, 3, 4, 6, 12.  No need for a hard‑coded table – we compute
       the divisors of 12 on the first call and store them statically. */
    static seq_t divisors[6];
    static size_t divcnt = 0;

    if (divcnt == 0) {
        /* Compute divisors of 12 by simple trial division. */
        for (idx_t i = 1; i <= 12; ++i) {
            if (12 % i == 0) {
                divisors[divcnt++] = (seq_t)i;
            }
        }
    }

    if (n == 0 || n > divcnt) {
        return 0;               /* sequence ends after the 6th term */
    }
    return divisors[n - 1];
}
