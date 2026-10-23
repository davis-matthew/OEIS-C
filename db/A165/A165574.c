/*
 * A165574  Legendre symbol (n,263).
 * https://oeis.org/A165574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 263, i16 n <= 263, i32 n <= 263, i64 n <= 263,
 *            i128 n <= 263, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Legendre symbol (n,263)
   a(0) = 0
   a(n) =  1  if n is a quadratic residue mod 263
          -1  if n is a non‑residue mod 263
          0  if 263 | n   */

#include <stdint.h>
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    const uint64_t p = 263ULL;          /* prime modulus */
    if (n == 0) return 0;              /* 263 | 0  → 0 */

    uint64_t r = n % p;                /* r ∈ [0,262]  */
    if (r == 0) return 0;              /* 263 | n  → 0 */

    /* Euler's criterion: (n|p) = n^((p-1)/2) mod p  */
    uint64_t res = powmod64(r, 131ULL, p);  /* 131 = (p-1)/2 */

    if (res == p - 1)                  /* p-1 ≡ -1 (mod p) */
        return (seq_t)-1;
    return (seq_t)1;
}
