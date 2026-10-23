/*
 * A115416  Imaginary part of (n + i)^n, with i=sqrt(-1).
 * https://oeis.org/A115416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0..⌊(n-1)/2⌋} (-1)^j * binom(n,2j+1) * n^(n-2j-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, a(n) is the imaginary part of (n+i)^n, with i=√‑1). */

#include <stdint.h>
#include "../utils/binom.h"

/* Fast exponentiation for arbitrary‐precision seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        exp >>= 1ULL;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t nseq   = (seq_t)n;                /* cast index to seq_t once */
    seq_t answer = (seq_t)0;

    /* Current power: n^(n-1) */
    seq_t cur_pow = pow_seq(nseq, n - 1);
    /* n^2 used to step down the powers by 2 each iteration */
    seq_t nsq = nseq * nseq;

    idx_t jmax = (n - 1) / 2;
    for (idx_t j = 0; j <= jmax; ++j) {
        idx_t k = 2 * j + 1;          /* binomial index (odd) */
        seq_t bin   = binom(n, k);    /* binomial coefficient */
        seq_t term  = bin * cur_pow;  /* contribution for this j */

        if (j & 1ULL) {
            /* (-1)^j is negative when j is odd */
            answer -= term;
        } else {
            answer += term;
        }

        /* Prepare n^(n-2(j+1)-1) for the next iteration,
           unless this was the last term. */
        if (j < jmax) cur_pow /= nsq;
    }

    return answer;
}
