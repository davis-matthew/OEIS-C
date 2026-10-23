/*
 * A008966  a(n) = 1 if n is squarefree, otherwise 0.
 * https://oeis.org/A008966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = 1 if n is squarefree, otherwise 0
 * Range:     u8 n <= 9907, u16 n <= 9907, u32 n <= 9907, u64 n <= 9907,
 *            u128 n <= 9907, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the squarefree indicator for n ≥ 1.
   Returns 1 if n is squarefree, 0 otherwise. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;            /* a(1) = 1, and 0 is considered squarefree here */

    idx_t temp = n;
    for (idx_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            size_t exp = 0;
            do {
                temp /= i;
                ++exp;
            } while (temp % i == 0);
            if (exp > 1)              /* found a square factor */
                return (seq_t)0;
        }
    }
    /* temp > 1 remains a prime factor with exponent 1 */
    return (seq_t)1;
}
