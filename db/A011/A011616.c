/*
 * A011616  Legendre symbol (n,167).
 * https://oeis.org/A011616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) is the Legendre symbol (n,167)
 * Range:     i8 n <= 1, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(n)=1 if n is a quadratic residue mod 167, -1 otherwise */
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    const uint64_t p = 167ull;          /* prime modulo */
    const uint64_t exp = 83ull;         /* (p-1)/2 */
    uint64_t nmod = n % p;              /* reduce n modulo p */

    if (nmod == 0ULL)           /* divisible by 167 */
        return (seq_t)0;

    seq_t r = mod_pow((seq_t)nmod, exp, (seq_t)p);  /* Euler's criterion */

    if (r == (seq_t)1)          /* residue */
        return (seq_t)1;

    /* otherwise r == p-1  → Legendre symbol -1 */
    return (seq_t)(-1);
}
