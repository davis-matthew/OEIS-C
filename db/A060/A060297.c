/*
 * A060297  Periodic part of decimal expansion of n / next prime > n.
 * https://oeis.org/A060297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 12, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A060297:
   The value a(n) is the repeating block of the decimal expansion of
   n divided by the first prime strictly greater than n.  If the
   division terminates (i.e. the next prime is 2 or 5) the result is 0. */

#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = next_prime(n);          /* first prime > n */
    if (p == 2 || p == 5)                /* terminating decimal */
        return 0;

    idx_t r = n;                         /* current remainder (0 < r < p) */
    seq_t res = 0;                       /* accumulating repeating digits */

    do {
        r *= 10;                         /* bring down next digit */
        idx_t d = r / p;                 /* digit 0..9 */
        r %= p;                          /* new remainder */
        res = res * 10 + (seq_t)d;      /* append digit to result */
    } while (r != n);                   /* repeat until remainder returns to start */

    return res;
}
