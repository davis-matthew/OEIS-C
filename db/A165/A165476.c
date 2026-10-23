/*
 * A165476  Legendre symbol (n,131071).
 * https://oeis.org/A165476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (n^65535 mod 131071) mapped to the Legendre symbol (0 →
 *            0, 1 → 1, 131070 → -1). Here 65535 = (131071–1)/2. The function
 *            returns a signed value –1, 0, or 1 for any n ≥ 0
 * Range:     i8 n <= 9522, i16 n <= 9522, i32 n <= 9522, i64 n <= 9522,
 *            i128 n <= 9522, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powmod64.h"

seq_t a(idx_t n) {
    const uint64_t MOD  = 131071ULL;   /* 2^17 – 1, the 6th Mersenne prime */
    const uint64_t EXP  = 65535ULL;    /* (MOD-1)/2 for Euler's criterion */

    /* Reduce the base modulo MOD before exponentiation. */
    uint64_t base = n % MOD;

    /* Compute base^EXP mod MOD using an efficient 64‑bit routine. */
    uint64_t r = powmod64(base, EXP, MOD);

    /* Map the reduced residue to the Legendre symbol. */
    if (r == 0)          return (seq_t)0;
    if (r == 1)          return (seq_t)1;
    /* r == MOD-1 corresponds to “-1” in the Legendre symbol. */
    return (seq_t)-1;
}
