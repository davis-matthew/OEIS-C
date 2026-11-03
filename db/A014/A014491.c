/*
 * A014491  a(n) = gcd(n, 2^n - 1).
 * https://oeis.org/A014491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = gcd(n, 2^n - 1)
 * Range:     u8 n <= 881, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9851 (2026-11-01)
 * Helpers:   gcd, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t r = powmod((seq_t)2, n, N); /* 2^n mod n */
    seq_t m = (r == 0) ? N - 1 : r - 1; /* (2^n - 1) mod n */
    return gcd(N, m);
}
