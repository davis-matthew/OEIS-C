/*
 * A053576  Smallest number whose Euler totient is divisible by 2^n.
 * https://oeis.org/A053576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that 2^n divides φ(m)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the first five Fermat primes and their 2‑adic valuations of p-1 */
    static const seq_t primes[5] = {(seq_t)3, (seq_t)5, (seq_t)17,
                                    (seq_t)257, (seq_t)65537};
    static const unsigned weights[5] = {1, 2, 4, 8, 16};

    seq_t best = 0;
    int best_set = 0;                /* flag to indicate that a candidate
                                        has already been selected */

    for (int mask = 0; mask < 32; ++mask) {
        unsigned w = 0;              /* total 2-adic weight of selected primes */
        seq_t  prod = 1;             /* product of the selected primes */

        /* compute product and total weight for this subset */
        for (int i = 0; i < 5; ++i) {
            if (mask & (1 << i)) {
                prod *= primes[i];
                w += weights[i];
            }
        }

        if (w > n) continue;        /* subset gives too much weight,
                                        cannot be combined with a 2‑power
                                        to get exactly n */

        /* remaining weight to reach n must be supplied by a power of 2:
           weight of 2^k is k‑1, so we need k‑1 = n – w, i.e. k = n – w + 1 */
        unsigned rem = (unsigned)(n - w);
        seq_t cur = prod;
        if (rem > 0) {
            /* multiply by 2^(rem+1).  We perform the shift one bit at a
               time so that the shift count never exceeds the type width.
               For all tested values the result fits the target type. */
            seq_t shift = 1;
            for (unsigned i = 0; i < rem + 1; ++i) {
                shift <<= 1;
            }
            cur *= shift;
        }

        if (!best_set || cur < best) {
            best = cur;
            best_set = 1;
        }
    }

    return best;
}
