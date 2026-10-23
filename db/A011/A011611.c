/*
 * A011611  Legendre symbol (n,139).
 * https://oeis.org/A011611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,139) = ((n%139)^69) mod 139, mapped to
 *            {-1,0,1} Recurrence: a(n) = -a(n-1) - a(n-2) - … - a(n-138) for
 *            n ≥ 139
 * Range:     i8 n <= 3, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Reduce n modulo 139 first */
    seq_t r = (seq_t)(n % (idx_t)139);
    if (r == (seq_t)0)
        return (seq_t)0;

    /* Compute r^69 mod 139 in a few multiplications. */
    seq_t v = mod_pow(r, 69ULL, (seq_t)139);

    /* The result v is either 1 or 138 (= -1 modulo 139). */
    if (v == (seq_t)1)
        return (seq_t)1;
    /* any non‑zero residue must be 138, i.e. Legendre symbol -1. */
    return (seq_t)(-1);
}
