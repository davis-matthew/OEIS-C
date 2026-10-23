/*
 * A151633  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20^n - (3*n + 1)*10^n + binomial(3*n+1,2)*4^n -
 *            binomial(3*n+1,3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binom.h"      /* seq_t binom(idx_t n, idx_t k); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  For n==0 we return 0 as a guard. */
    if (n == 0)
        return 0;

    /* Powers */
    seq_t pow20 = big_pow((seq_t)20, (seq_t)n);
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow4  = big_pow((seq_t)4,  (seq_t)n);

    /* Binomial coefficients */
    idx_t t = 3*n + 1;
    seq_t binom2 = binom(t, 2);
    seq_t binom3 = binom(t, 3);

    /* Intermediate terms using only seq_t arithmetic */
    seq_t term2 = seq_mul(pow10, (seq_t)t);   /* (3*n+1)*10^n */
    seq_t term3 = seq_mul(binom2, pow4);      /* binom(3*n+1,2)*4^n */

    /* rearrange to keep all intermediate results non‑negative:
       a(n) = (20^n + term3) - (term2 + binom3) */
    seq_t sum1 = pow20 + term3;
    seq_t sum2 = term2 + binom3;
    return sum1 - sum2;
}
