/*
 * A042655  Denominators of continued fraction convergents to sqrt(857).
 * https://oeis.org/A042655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 16237136*a(n-17) + a(n-34) for n > 33 (the same sequence
 *            is obtained by iterating the continued‑fraction algorithm for
 *            sqrt(857).)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 23, u64 n <= 48, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042655: Denominators of continued‑fraction convergents to sqrt(857) */
/* Initial values: a(0)=1, a(1)=3, a(2)=4, a(3)=7, ... as given in OEIS. */

#include <stddef.h>   /* for idx_t */

seq_t a(idx_t n)
{
    /* Handle the zero index explicitly. */
    if (n == 0) {
        return (seq_t)1U;
    }

    /* Constants for the continued fraction of sqrt(857). */
    const uint64_t D   = 857ULL;   /* the radicand */
    const uint64_t a0  = 29ULL;    /* floor(sqrt(857)) */

    /* Current state of the continued‑fraction expansion. */
    uint64_t m = 0ULL;      /* m_k */
    uint64_t d = 1ULL;      /* d_k */
    uint64_t a_k = a0;      /* a_k */

    /* Recurrence for convergents: p_{-2}=0, p_{-1}=1, q_{-2}=1, q_{-1}=0. */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;

    /* Iterate from k = 0 up to k = n, computing the denominator q_k. */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t qk = (seq_t)a_k * q_prev1 + q_prev2;   /* q_k = a_k*q_{k-1}+q_{k-2} */

        if (k == n) {
            return qk;
        }

        seq_t pk = (seq_t)a_k * p_prev1 + p_prev2;   /* p_k = a_k*p_{k-1}+p_{k-2} */

        /* Advance to the next partial quotient. */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (a0 + m) / d;   /* floor((a0 + m)/d) */

        /* Shift the recurrence stores for the next iteration. */
        p_prev2 = p_prev1; p_prev1 = pk;
        q_prev2 = q_prev1; q_prev1 = qk;
    }

    /* The loop always returns before reaching here. */
    return (seq_t)0;
}
