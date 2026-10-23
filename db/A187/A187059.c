/*
 * A187059  The exponent of highest power of 2 dividing the product of the
 *          elements of the n-th row of Pascal's triangle (A001142).
 * https://oeis.org/A187059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (n+1)*(n - popcount(n)) - 2*((n*(n+1)/2) - Σ_{k=0}^n
 *            popcount(k))
 * Range:     u8 n <= 63, u16 n <= 7167, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sum of bit counts from 0 to n */
    seq_t sumPop = 0;
    for (idx_t b = 0;; ++b) {
        idx_t block = (idx_t)1 << b;
        if (block > n) break;
        idx_t period = block << 1;
        idx_t full   = (n + 1) / period;
        idx_t ones   = full * block;
        idx_t rem    = (n + 1) % period;
        if (rem > block) ones += rem - block;
        sumPop += (seq_t)ones;
    }

    seq_t n_seq     = (seq_t)n;
    seq_t n_plus1   = (seq_t)(n + 1);
    seq_t pop_n     = (seq_t)__builtin_popcountll((unsigned long long)n);
    seq_t v2n       = n_seq - pop_n;                     /* ν₂(n!) = n - popcount(n) */
    seq_t term1     = n_plus1 * v2n;                     /* (n+1)*(n - popcount(n)) */
    seq_t half      = (n_seq * n_plus1) / 2;             /* n*(n+1)/2  */
    seq_t n2sum     = half - sumPop;                    /* n(n+1)/2 - Σ popcount(k) */
    seq_t term2     = 2 * n2sum;                        /* 2 * that */
    seq_t res       = term1 - term2;                    /* final exponent */

    return res;
}
