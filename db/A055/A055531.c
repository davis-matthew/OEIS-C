/*
 * A055531  Number of labeled order relations on n nodes in which longest
 *          chain has 2 nodes.
 * https://oeis.org/A055531
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n-1} binomial(n,k)*(2^(n-k)-1)^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility headers */
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */
#include "../utils/pow2.h"       /* provides seq_t pow2(idx_t)       */
#include "../utils/big_pow.h"    /* provides seq_t big_pow(seq_t, seq_t) */

/* Compute the nth term of A055531 (offset 2). */
seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* iterate k = 1 .. n-1 */
    for (idx_t k = 1; k <= n - 1; ++k) {
        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* compute 2^(n-k) */
        seq_t two_pow = pow2(n - k);

        /* compute (2^(n-k) - 1)^k */
        seq_t base = two_pow - (seq_t)1;
        seq_t power = big_pow(base, (seq_t)k);

        /* term = bin * power */
        seq_t term = bin * power;

        /* accumulate */
        sum += term;
    }

    return sum;
}
