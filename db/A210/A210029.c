/*
 * A210029  Number of sequences over the alphabet of n symbols of length 2n
 *          which have n distinct symbols. Also number of placements of 2n
 *          balls into n cells where no cell is empty.
 * https://oeis.org/A210029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{v=0..n-1} (-1)^v * binomial(n,v) * (n-v)^(2n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* Two times n (the exponent). */
    idx_t two_n = n + n;

    seq_t pos = 0;   /* Sum of terms with even v. */
    seq_t neg = 0;   /* Sum of terms with odd  v. */

    for (idx_t v = 0; v < n; ++v) {
        seq_t choose = binom(n, v);              /* C(n, v) */
        seq_t base   = (seq_t)(n - v);           /* n - v   */
        seq_t pow_val = seq_pow(base, two_n);    /* (n-v)^(2n) */
        seq_t term = choose * pow_val;           /* C(n,v)*(n-v)^(2n) */

        if ((v & 1) == 0)                       /* even v → + */
            pos += term;
        else                                    /* odd  v → - */
            neg += term;
    }

    /* For all n >= 1 the result is non‑negative. */
    return pos - neg;
}
