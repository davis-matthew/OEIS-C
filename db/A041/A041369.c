/*
 * A041369  Denominators of continued fraction convergents to sqrt(199).
 * https://oeis.org/A041369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n‑th convergent of sqrt(199).
 *            Recurrence: k_{-2} = 1, k_{-1} = 0 k_n = A_n * k_{n-1} + k_{n-2}
 *            where A_n are the partial quotients of the continued fraction of
 *            sqrt(199): A_0 = 14, and for n ≥ 1, A_n cycles with period 20:
 *            {9,2,1,2,2,5,4,1,1,13,1,1,4,5,2,2,1,2,9,28}
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* partial quotients for sqrt(199) after the initial term */
    static const uint64_t period[20] = {
        9ULL, 2ULL, 1ULL, 2ULL, 2ULL, 5ULL, 4ULL, 1ULL,
        1ULL, 13ULL, 1ULL, 1ULL, 4ULL, 5ULL, 2ULL, 2ULL,
        1ULL, 2ULL, 9ULL, 28ULL
    };

    seq_t prev2 = (seq_t)1; /* k_{-2} */
    seq_t prev1 = (seq_t)0; /* k_{-1} */
    seq_t cur   = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)14; /* a_0 = floor(sqrt(199)) */
        else
            ai = (seq_t)period[(i - 1) % 20];
        cur = ai * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur; /* k_n, the requested denominator */
}
