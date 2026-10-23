/*
 * A151656  Number of permutations of 7 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36^n - (7*n + 1)*8^n + 7*n*(7*n + 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation: returns base^exp.
   Works for any integer type seq_t supported by the build. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; n==0 is unused but handled safely */
        return (seq_t)0;

    seq_t ns   = (seq_t)n;           /* cast index to seq_t */
    seq_t pow36 = pow_seq((seq_t)36, n);
    seq_t pow8  = pow_seq((seq_t)8,  n);

    seq_t term1 = pow36;
    seq_t term2 = ((seq_t)7 * ns + (seq_t)1) * pow8;
    seq_t term3 = (seq_t)7 * ns * ((seq_t)7 * ns + (seq_t)1);
    term3 /= (seq_t)2;               /* 7*n*(7*n+1)/2 */

    seq_t res = term1 - term2 + term3;
    return res;
}
