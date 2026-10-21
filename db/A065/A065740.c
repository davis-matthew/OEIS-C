/*
 * A065740  Largest square <= n^n.
 * https://oeis.org/A065740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n^n))^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, offset is 1 */

    /* cast index to seq_t to avoid intermediate overflow on the index type */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)n;

    /* compute n^n */
    seq_t pow_n = big_pow(base, exp);

    /* largest integer <= sqrt(n^n) */
    seq_t root = floor_sqrt(pow_n);

    /* return the square of that integer */
    return root * root;
}
