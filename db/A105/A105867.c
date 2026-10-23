/*
 * A105867  A generalized Chebyshev transform of the Jacobsthal numbers.
 * https://oeis.org/A105867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^k * binom(n-k, k) * J(n-2k) where J(m) =
 *            (2^m – (‑1)^m) / 3 is the Jacobsthal number
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* Helper functions --------------------------------------------------- */
/*  binom(N, K)    – binomial coefficient C(N, K)                      */
/*  pow2(exp)      – 2^exp                                            */
/* ------------------------------------------------------------------ */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;            /* index for Jacobsthal number */
        seq_t pow2k = pow2(k);         /* 2^k                */
        seq_t pow2m = pow2(m);         /* 2^m                */

        /* Jacobsthal J(m) = (2^m – (‑1)^m) / 3 */
        seq_t J;
        if ((m & 1ULL) == 0)           /* m even: (2^m – 1) / 3 */
            J = (pow2m - 1) / 3;
        else                           /* m odd : (2^m + 1) / 3 */
            J = (pow2m + 1) / 3;

        seq_t bin = binom(n - k, k);   /* C(n‑k, k)         */
        seq_t term = bin * J * pow2k;  /* product of all parts */

        result += term;                /* accumulate sum     */
    }
    return result;
}
