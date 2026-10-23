/*
 * A040633  Continued fraction for sqrt(659).
 * https://oeis.org/A040633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040633:  Continued fraction for sqrt(659)
    Multiplicative definition:
      a(0) = 25
      a(2^e) = 2  (for any e ≥ 1)
      a(3^e) = 25 (for any e ≥ 1)
      a(p^e) = 1  (for any prime p ≥ 5, any e ≥ 1)
    Thus a(n) = 2^{[2|n]} · 25^{[3|n]} where [P] is 1 if P is true, 0 otherwise.
*/

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)25;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Handle factor 2 */
    if ((m & 1) == 0) {
        result *= (seq_t)2;
        while ((m % 2) == 0) {
            m /= 2;
        }
    }

    /* Handle factor 3 */
    if ((m % 3) == 0) {
        result *= (seq_t)25;
        while ((m % 3) == 0) {
            m /= 3;
        }
    }

    /* All other prime factors contribute 1, so can be ignored */
    return result;
}
