/*
 * A010970  a(n) = binomial(n,17).
 * https://oeis.org/A010970
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,17)
 * Range:     u8 n <= 19, u16 n <= 22, u32 n <= 34, u64 n <= 105,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = C(n,17) for n ≥ 17.  Results are exact as long as they
 * fit in seq_t; for larger values seq_t must support arbitrary precision.
 * The algorithm multiplies the numerator terms one by one and divides by
 * the corresponding denominator, using a 128‑bit intermediate when the
 * compiler provides __int128 to avoid overflow on 64‑bit seq_t.  */

seq_t a(idx_t n)
{
    if (n < 17) return 0;          /* sequence is defined only for n ≥ 17 */

    seq_t result = 1;

    /* multiply result by each numerator term and divide by the
     * corresponding denominator term.  This respects the exact
     * integer nature of the binomial coefficient. */
    for (idx_t i = 1; i <= 17; ++i)
    {
        seq_t num = (seq_t)(n - 17 + i);

#ifdef __SIZEOF_INT128__
        /* Use 128‑bit intermediate for 64‑bit seq_t to avoid overflow. */
        unsigned __int128 tmp = (unsigned __int128)result * (unsigned __int128)num;
        result = (seq_t)(tmp / i);
#else
        /* Rely on seq_t's own multiplication to handle big integers. */
        result = (result * num) / i;
#endif
    }

    return result;
}
