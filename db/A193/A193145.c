/*
 * A193145  Self-composition of binary encoding of GF(2) polynomial.
 * https://oeis.org/A193145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(f(x)) where f(x)=∑b_i x^i is the binary expansion of n,
 *            evaluated in the field GF(2). The result is the binary
 *            polynomial again encoded as an integer
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   poly_mul_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/poly_mul_seq_t.h"

/* a(n) – self‑composition of the binary polynomial of n in GF(2). */
seq_t a(idx_t n)
{
    seq_t f = (seq_t)n;               /* polynomial f(x) */
    seq_t result = 0;
    seq_t power   = 1;                /* f^i  (i will grow from 0) */

    if (f == 0)                      /* f(x)=0 ⇒ f(f(x))=0 */
        return 0;

    /* determine the number of bits needed to represent n */
    idx_t deg = 0;
    for (seq_t temp = f; temp > 0; ++deg)
        temp >>= 1;

    for (idx_t i = 0; i < deg; ++i) {
        /* if the i‑th bit of n is set, add f^i to the result */
        if (f & ((seq_t)1 << i))
            result ^= power;
        /* advance power to f^(i+1) */
        power = poly_mul_seq_t(power, f);
    }
    return result;
}
