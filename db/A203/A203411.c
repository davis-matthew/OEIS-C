/*
 * A203411  Discriminant of the cyclotomic binomial period polynomial for an
 *          odd prime.
 * https://oeis.org/A203411
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^((prime(n)-3)/2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* offset starts at 2; undefined for n < 2 */
        return 0;
    }
    uint32_t p = get_prime(n);          /* nth prime, 1‑based */
    idx_t    exp = (idx_t)(p - 3) / 2;   /* integer exponent */
    return pow_uint(p, exp);            /* p^exp as seq_t */
}
