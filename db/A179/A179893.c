/*
 * A179893  a(n) = 3/2 * (prime(n)-1).
 * https://oeis.org/A179893
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3/2*(prime(n)-1)
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;  /* behaviour below the offset is irrelevant */
    }

    /* obtain the nth prime; n is 1‑based in get_prime */
    uint32_t p = get_prime(n);

    /* compute (3/2)*(p-1).  The division is performed first so that
       the intermediate does not overflow a 64‑bit integer.
       Casting to seq_t occurs last; if seq_t is smaller than the
       result the value will wrap exactly as required. */
    uint64_t tmp = ((uint64_t)p - 1) / 2;  /* (p-1)/2 is integer */
    tmp = tmp * 3;                         /* multiply by 3 */

    return (seq_t)tmp;
}
