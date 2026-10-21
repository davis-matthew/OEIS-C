/*
 * A053181  Composite numbers ending in 9.
 * https://oeis.org/A053181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th composite number of the form 10*k + 9 (k ≥ 0)
 * Range:     u8 n <= 13, u16 n <= 4931, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

/* External integer types are provided by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    idx_t count = 0;
    idx_t k = 0;
    seq_t val;

    for (;; ++k) {
        val = ((seq_t)10 * k) + (seq_t)9;  /* candidate 10k+9 */
        if (!isPrimeGeneric(val)) {        /* composite check */
            ++count;
            if (count == n)
                return val;
        }
    }
}
