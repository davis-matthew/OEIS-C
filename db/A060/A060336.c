/*
 * A060336  Number of n X n {-1,0,1} matrices modulo rows permutation (by
 *          symmetry this is the same as the number of {-1,0,1} matrices
 *          modulo columns permutation), i.e., the number of equivalence
 *          classes where two matrices A and B are equivalent if one of them
 *          is the result of permuting the rows of the other.
 * https://oeis.org/A060336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(3^n + n - 1, n) (OEIS A060336)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the binomial coefficient multiplicatively to avoid large
   factorials.  The calculation is valid for any seq_t (uint8_t …,
   unsigned __int128, arbitrary‑precision class).                */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 3^n  (base 3) */
    seq_t pow3 = 1;
    seq_t base = 3;
    idx_t e = n;
    while (e != 0) {
        if (e & 1) {
            pow3 = pow3 * base;
        }
        base = base * base;
        e >>= 1;
    }

    /* N = 3^n + n - 1  (as seq_t) */
    seq_t N = pow3 + ((seq_t)n) - 1;

    /* compute binomial(N, n) multiplicatively */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = N - ((seq_t)n) + (seq_t)i;
        res = res * numerator;
        res = res / (seq_t)i;   /* division is exact */
    }
    return res;
}
