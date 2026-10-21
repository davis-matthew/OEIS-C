/*
 * A075895  Rounded sum of first n odd integers squared divided by n-th prime.
 * https://oeis.org/A075895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n*(4*n*n - 1) / (3 * prime(n))) for n ≥ 2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1161, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0 */
    if (n < 2)
        return 0;

    /* Work in seq_t to avoid overflow and keep exactness */
    seq_t nt   = (seq_t)n;                          /* n as seq_t */
    seq_t sq   = ((seq_t)4 * nt * nt) - 1;          /* 4n² - 1 */
    seq_t num  = nt * sq;                           /* n*(4n² - 1) */

    seq_t p    = (seq_t)get_prime(n);               /* nth prime, 1‑indexed */
    seq_t den  = 3 * p;                             /* 3 * prime(n) */

    /* Integer rounding: (num + den/2) / den */
    seq_t res = (num + den / 2) / den;
    return res;
}
