/*
 * A106514  Expansion of (1-x)/((1-2*x)*(1-2*x-x^2)).
 * https://oeis.org/A106514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Pell(n+3) - Pell(n+2) - 2^(n+1) (Pell(n) =
 *            2*Pell(n-1)+Pell(n-2), Pell(0)=0, Pell(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Compute Pell(n+3) and Pell(n+2) with a simple recurrence */
    seq_t p_prev2 = 0;          /* Pell(0) */
    seq_t p_prev1 = 1;          /* Pell(1) */
    seq_t p_curr;

    if (n + 3 >= 2) {
        for (idx_t i = 2; i <= n + 3; ++i) {
            /* P(i) = 2*P(i-1) + P(i-2) */
            p_curr   = (seq_t)2 * p_prev1 + p_prev2;
            p_prev2 = p_prev1;
            p_prev1 = p_curr;
        }
    }
    /* After loop: p_prev1 = Pell(n+3), p_prev2 = Pell(n+2) */

    /* 2^(n+1) */
    seq_t pow2 = seq_pow((seq_t)2, n + 1);

    /* Compute a(n) */
    seq_t result = p_prev1;
    result -= p_prev2;
    result -= pow2;

    return result;
}
