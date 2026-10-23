/*
 * A165573  Legendre symbol (n,257).
 * https://oeis.org/A165573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,257)
 * Range:     i8 n <= 0, i16 n <= 257, i32 n <= 257, i64 n <= 257,
 *            i128 n <= 257, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Reduce n modulo the prime 257 first. */
    idx_t n_mod = n % 257;
    if (n_mod == 0)
        return 0;

    /* Euler's criterion: (n|p) = n^((p-1)/2) mod p,  p=257, (p-1)/2 = 128 */
    seq_t r = mod_pow((seq_t)n_mod, (uint64_t)128, (seq_t)257);

    /* In modulo 257: residue yields 1, non‑residue yields 256 (= -1). */
    return r == (seq_t)1 ? (seq_t)1 : (seq_t)-1;
}
