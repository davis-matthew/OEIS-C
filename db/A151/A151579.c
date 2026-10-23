/*
 * A151579  Number of permutations of 1..n arranged in a circle with exactly 6
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151579
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(6^(n-1) - n*5^(n-1) + binom(n,2)*4^(n-1) -
 *            binom(n,3)*3^(n-1) + binom(n,4)*2^(n-1) - binom(n,5))
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute power with exponentiation by squaring */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Compute binomial coefficient C(n,k) */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Computes the OEIS sequence A151579. */
seq_t a(idx_t n)
{
    if (n < (idx_t)6) return (seq_t)0;   /* values below the offset are undefined */

    /* Common exponent (n-1). */
    idx_t exp = n - (idx_t)1;

    /* Powers of small constants. */
    seq_t pow6 = pow_seq((seq_t)6, exp);
    seq_t pow5 = pow_seq((seq_t)5, exp);
    seq_t pow4 = pow_seq((seq_t)4, exp);
    seq_t pow3 = pow_seq((seq_t)3, exp);
    seq_t pow2 = pow_seq((seq_t)2, exp);

    /* Build the alternating sum using unsigned arithmetic. */
    seq_t term = pow6;
    term += binom_seq(n, (idx_t)2) * pow4;
    term += binom_seq(n, (idx_t)4) * pow2;
    term -= (seq_t)n * pow5;
    term -= binom_seq(n, (idx_t)3) * pow3;
    term -= binom_seq(n, (idx_t)5);

    return (seq_t)n * term;  /* final multiplication by n */
}
