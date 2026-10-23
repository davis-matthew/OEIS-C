/*
 * A166629  Totally multiplicative sequence with a(p) = 8p for prime p.
 * https://oeis.org/A166629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 8^Ω(n), where Ω(n) is the total number of prime
 *            factors of n counted with multiplicity. The sequence is
 *            multiplicative with a(p^e) = (8p)^e
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n ≥ 1; bigomega(1) == 0, so the product is initialized correctly. */
    seq_t result = (seq_t)n;
    seq_t omega = bigomega(n);
    if (omega != 0)
    {
        /* Convert the (small) exponent to a rank type before calling pow_uint. */
        idx_t exp = (idx_t)(uint64_t)omega;
        result *= pow_uint(8, exp);
    }
    return result;
}
