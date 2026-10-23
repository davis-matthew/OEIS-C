/*
 * A103730  Negative of column k=3 sequence of array A103728.
 * https://oeis.org/A103730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p^3 - 7p^2 + 17p - 17) / 6, where p is the (n+3)-th
 *            prime
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The (n+3)-th prime */
    seq_t p = get_prime(n + 3);

    /* Compute p^2 and p^3 */
    seq_t p2 = p * p;        /* p² */
    seq_t p3 = p2 * p;       /* p³ */

    /* Evaluate the numerator p³ - 7p² + 17p - 17 */
    seq_t num = p3;
    num -= (seq_t)7 * p2;    /* -7p² */
    num += (seq_t)17 * p;    /* +17p */
    num -= (seq_t)17;        /* -17 */

    /* Divide by 6 (= 3!) */
    return num / (seq_t)6;
}
