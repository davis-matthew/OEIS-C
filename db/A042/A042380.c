/*
 * A042380  Numerators of continued fraction convergents to sqrt(717).
 * https://oeis.org/A042380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(717)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Integer part of sqrt(717) */
    const seq_t a0 = 26;

    /* Periodic part of the continued fraction expansion of sqrt(717) */
    static const uint64_t period[] = {
        1ULL, 3ULL, 2ULL, 12ULL, 1ULL,
        16ULL, 1ULL, 12ULL, 2ULL, 3ULL,
        1ULL, 52ULL
    };
    const seq_t period_len = sizeof period / sizeof period[0];

    /* Recurrence variables for convergents: P[-2]=0, P[-1]=1; Q[-2]=1, Q[-1]=0 */
    seq_t P_m2 = 0, P_m1 = 1;
    seq_t Q_m2 = 1, Q_m1 = 0;
    seq_t Pk, Qk;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t aik;
        if (k == 0)
            aik = a0;                                /* a0 */
        else
            aik = (seq_t)period[(size_t)((k - 1) % period_len)];

        /* Compute the k-th convergent using the recurrence */
        Pk = aik * P_m1 + P_m2;
        Qk = aik * Q_m1 + Q_m2;

        /* Shift previous values */
        P_m2 = P_m1;
        P_m1 = Pk;
        Q_m2 = Q_m1;
        Q_m1 = Qk;
    }

    return P_m1;  /* Numerator of the n-th convergent */
}
