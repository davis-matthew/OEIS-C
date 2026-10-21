/*
 * A060435  Number of functions f: {1,2,...,n} -> {1,2,...,n} with even cycles
 *          only.
 * https://oeis.org/A060435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0}^{⌊(n-2)/2⌋} (k+1)/2^{2k+1} *
 *            binomial(2k+2, k+1) * n^{n-2-2k} / (n-2-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   count_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the summand term by cancelling powers of 2 before multiplication to avoid
   overflow when seq_t is a 64‑bit unsigned integer.  It works for arbitrary‑precision builds because
   all operations are performed on seq_t values.  */

#include <stdint.h>
#include "../utils/count_twos.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    idx_t maxk = (n - 2) / 2;
    seq_t sum = 0;

    /* Pre‑compute n^exp where exp = n-2 */
    seq_t npow = 1;
    for (idx_t i = 0; i < n - 2; ++i) npow *= (seq_t)n;

    seq_t n_sq = (seq_t)n * (seq_t)n;       /* 2^2 */
    seq_t ratio = (seq_t)(n - 1);           /* (n-1)! / (n-2)!  */
    seq_t binom = (seq_t)2;                 /* C(2,1)            */
    seq_t pow2 = (seq_t)2;                  /* 2^(1)             */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = 1;
        idx_t rem = 2 * k + 1;              /* exponent of 2 to divide */

        /* Component 1: ratio */
        seq_t comp = ratio;
        idx_t tw = count_twos(comp);
        if (tw > rem) tw = rem;
        comp >>= tw;
        rem -= tw;
        term *= comp;

        /* Component 2: (k+1) */
        comp = (seq_t)(k + 1);
        tw = count_twos(comp);
        if (tw > rem) tw = rem;
        comp >>= tw;
        rem -= tw;
        term *= comp;

        /* Component 3: binom */
        comp = binom;
        tw = count_twos(comp);
        if (tw > rem) tw = rem;
        comp >>= tw;
        rem -= tw;
        term *= comp;

        /* Component 4: npow */
        comp = npow;
        tw = count_twos(comp);
        if (tw > rem) tw = rem;
        comp >>= tw;
        rem -= tw;
        term *= comp;

        /* At this point rem must be zero (numerator has at least 2k+1 twos) */
        sum += term;

        /* Prepare for next k */
        if (k == maxk) break;

        /* Update ratio: multiply by (r)*(r-1), where r = n-2-2k */
        seq_t a1 = (seq_t)(n - 2 - 2 * k - 1);
        seq_t a2 = (seq_t)(n - 2 - 2 * k);
        ratio *= a1;
        ratio *= a2;

        /* Update binom: binom = binom * (2k+4)(2k+3) / ((k+2)^2) */
        seq_t num = (seq_t)(2 * k + 4) * (seq_t)(2 * k + 3);
        seq_t den = (seq_t)(k + 2) * (seq_t)(k + 2);
        binom *= num;
        binom /= den;

        /* Update pow2: multiply by 4 */
        pow2 <<= 2;                 /* 2^(2k+3) for next k */

        /* Update npow: divide by n^2 */
        npow /= n_sq;
    }
    return sum;
}
