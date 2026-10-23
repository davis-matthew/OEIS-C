/*
 * A011612  Legendre symbol (n,149).
 * https://oeis.org/A011612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,149) = n^74 (mod 149)
 * Range:     i8 n <= 3, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache the 149 possible values of the symbol */
    static seq_t cache[149];
    static int initialized = 0;

    if (!initialized) {
        /* pre‑compute a(i) for i = 0..148 */
        const seq_t mod = 149;
        for (idx_t i = 0; i < 149; ++i) {
            seq_t base = (seq_t)i;
            /* binary exponentiation of base^74 mod 149 */
            seq_t res  = 1;
            seq_t b    = base % mod;
            unsigned exp = 74;  /* exponent fits in an unsigned int */
            while (exp) {
                if (exp & 1) {
                    res = (res * b) % mod;
                }
                b = (b * b) % mod;
                exp >>= 1;
            }
            /* interpret the residue as the Legendre symbol */
            if (res == 0)
                cache[i] = 0;
            else if (res == 1)
                cache[i] = 1;
            else
                cache[i] = -1;   /* res must be 148 (= -1 mod 149) */
        }
        initialized = 1;
    }

    /* the value depends only on n mod 149 */
    idx_t idx = n % 149;
    return cache[idx];
}
