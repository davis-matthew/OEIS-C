/*
 * A164292  Binary sequence identifying the twin primes (characteristic
 *          function of twin primes: 1 if n is a twin prime else 0).
 * https://oeis.org/A164292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) * ceil(( c(n+2) + c(n-2) ) / 2) where c(k)=1 if k is
 *            prime, else 0. a(1)=0, a(2)=0, a(3)=1 ... This implements the
 *            twin‑prime binary characteristic
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 2)
        return (seq_t)0;

    /* n must be prime to contribute */
    if (!is_prime(n))
        return (seq_t)0;

    bool pplus2 = false;
    bool pminus2 = false;

    /* Avoid overflow on unsigned addition */
    if (n <= UINT64_MAX - 2)
        pplus2 = is_prime(n + 2);

    /* Avoid underflow: only check n-2 if n>2 */
    if (n > 2)
        pminus2 = is_prime(n - 2);

    /* ceil( (pplus2 + pminus2) / 2 )   for 0/1 values
       equals (pplus2 + pminus2 + 1) / 2   in integer division   */
    int half = (pplus2 + pminus2 + 1) >> 1;   /* shift is equivalent to /2 */
    return (seq_t)half;
}
