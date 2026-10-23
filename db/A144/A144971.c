/*
 * A144971  Integers of the form sum_{i=2521..j} i/(i-2520) for any upper
 *          limit j.
 * https://oeis.org/A144971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=2521..j} i/(i-2520) for the integer j that makes
 *            the sum an integer. Compute the nth such integer
 * Range:     u8 none, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t h_num = 0;                 /* numerator of H_m */
    seq_t h_den = 1;                 /* denominator of H_m (reduced) */
    idx_t m = 0;                     /* current m = j-2520 */
    idx_t count = 0;                 /* number of integer terms found */

    while (count < n) {
        ++m;                                  /* next term in the harmonic sum */

        /* H_m = H_{m-1} + 1/m */
        seq_t new_num = h_num * (seq_t)m + h_den;
        seq_t new_den = h_den * (seq_t)m;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        h_num = new_num / g;
        h_den = new_den / g;

        /* Check if the integer condition holds */
        if (h_den <= (seq_t)2520) {
            if (2520ULL % (uint64_t)h_den == 0) {
                seq_t term = (seq_t)m + ((2520ULL / (uint64_t)h_den) * h_num);
                ++count;
                if (count == n) return term;
            }
        }
    }

    /* This line should never be reached */
    return 0;
}
