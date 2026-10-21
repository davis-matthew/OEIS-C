/*
 * A062842  Maximum number of zeros in the representation of n in any base.
 * https://oeis.org/A062842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search over all bases b ≥ 2 for a given n.
   For each base, count the zero digits in the base‑b representation
   of n and return the maximum count.  This matches the definition
   of OEIS A062842: a(n) = max_{b≥2} #{digits zero in representation of n in b}. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0 by convention */
        return 0;

    seq_t best = 0;
    seq_t nseq = (seq_t)n;     /* convert to seq_t once */

    for (idx_t b = 2; b <= n; ++b) {
        seq_t val   = nseq;
        seq_t zeros = 0;
        while (val > 0) {
            seq_t rem = val % b;   /* remainder digit */
            if (rem == 0)
                ++zeros;
            val = val / b;
        }
        if (zeros > best)
            best = zeros;
    }
    return best;
}
