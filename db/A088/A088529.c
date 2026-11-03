/*
 * A088529  Numerator of Bigomega(n)/Omega(n).
 * https://oeis.org/A088529
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of bigomega(n) / omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9852 (2026-11-01)
 * Helpers:   bigomega, gcd, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 2. For other arguments we return 0. */
    if (n < 2)
        return 0;

    seq_t B = bigomega(n);                /* total prime factors with multiplicity */
    int   O_int = omega(n); /* number of distinct prime factors   */
    seq_t O = (seq_t)O_int;                /* convert to seq_t for arithmetic    */

    seq_t g = gcd(B, O);                   /* greatest common divisor           */
    return B / g;                          /* numerator of the reduced fraction */
}
