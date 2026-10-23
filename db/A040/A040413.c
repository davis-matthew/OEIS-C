/*
 * A040413  Continued fraction for sqrt(434).
 * https://oeis.org/A040413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function:
   a(0) = 20
   a(1) = 1
   a(2)     = 4
   a(2^e)   = 40   (e ≥ 2)
   a(p^e)   = 1    (p odd prime)   */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;
    if (n == 1)
        return (seq_t)1;

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* Handle the prime factor 2 */
    if ((tmp & 1ULL) == 0ULL) {
        unsigned int e = 0;
        while ((tmp & 1ULL) == 0ULL) {
            tmp >>= 1;
            ++e;
        }
        if (e == 1)
            result *= (seq_t)4;
        else /* e >= 2 */ 
            result *= (seq_t)40;
    }

    /* Handle odd prime factors (contribute 1, so no effect) */
    for (idx_t p = 3; (seq_t)p * (seq_t)p <= (seq_t)tmp; p += 2) {
        while (tmp % p == 0) {
            tmp /= p;
            /* odd prime factor: value 1 => no change to result */
        }
    }
    /* Remaining tmp > 1 is an odd prime; contributes 1 */

    return result;
}
