/*
 * A031974  1 repeated prime(n) times.
 * https://oeis.org/A031974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (10^prime(n) - 1) / 9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 behavior is irrelevant */
    if (n == 0)
        return 0;

    /* Obtain the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    /* Compute 10^p */
    seq_t val = pow10((idx_t)p);

    /* (10^p - 1) / 9 */
    val = val - 1;
    val = val / 9;

    return val;
}
