/*
 * A151634  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 4 adjacent element pairs in decreasing order.
 * https://oeis.org/A151634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35^n - (3*n + 1)*20^n + binomial(3*n+1,2)*10^n -
 *            binomial(3*n+1,3)*4^n + binomial(3*n+1,4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* n is at least 1 for valid calls. */
    if (n < 3) return 0;            /* a(1)=a(2)=0 by the formula */

    /* Compute the powers */
    seq_t pow35 = big_pow((seq_t)35, (seq_t)n);
    seq_t pow20 = big_pow((seq_t)20, (seq_t)n);
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow4  = big_pow((seq_t)4 , (seq_t)n);

    /* Binomials: m = 3*n + 1 */
    idx_t m = 3 * n + 1;
    seq_t c2 = binom(m, 2);
    seq_t c3 = binom(m, 3);
    seq_t c4 = binom(m, 4);

    /* Positive part: 35^n + C(3*n+1,2)*10^n + C(3*n+1,4) */
    seq_t pos = pow35;
    pos += seq_mul(c2, pow10);
    pos += c4;

    /* Negative part: (3*n+1)*20^n + C(3*n+1,3)*4^n */
    seq_t coeff = (seq_t)(3 * n + 1);
    seq_t neg = seq_mul(coeff, pow20);
    neg += seq_mul(c3, pow4);

    /* The formula guarantees the result is non‑negative for n>=3. */
    return pos - neg;
}
