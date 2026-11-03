/*
 * A062509  a(n) = n^omega(n).
 * https://oeis.org/A062509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^omega(n)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 329, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 998 (2026-11-01)
 * Helpers:   ipow, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1, but guard against 0 */
        return 0;

    int k = omega((uint64_t)n); /* number of distinct primes of n */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)k;

    return ipow(base, exp);  /* compute n^k */
}
