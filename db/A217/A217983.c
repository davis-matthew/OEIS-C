/*
 * A217983  If n = floor(p/2) * p^e, for some (by necessity unique) prime p
 *          and exponent e > 0, then a(n) = p, otherwise a(n) = 1.
 * https://oeis.org/A217983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p if n = floor(p/2) * p^e for some prime p and e > 0,
 *            else 1
 * Range:     u8 n <= 3855, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)                /* n = 1 => a(1) = 1 */
        return (seq_t)1;

    idx_t orig = n;          /* preserve original n */
    idx_t temp = n;          /* temp will be divided during factorisation */

    /* test all prime factors found by trial division */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) continue;

        /* count the exponent of p in n */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* compute p^e using seq_t arithmetic */
        seq_t pseq = (seq_t)p;
        seq_t p_pow = 1;
        for (idx_t i = 0; i < e; ++i)
            p_pow *= pseq;

        /* remaining factor after removing p^e */
        seq_t rest = (seq_t)orig / p_pow;

        /* floor(p/2) */
        seq_t p2 = (p == 2) ? (seq_t)1 : ((seq_t)(p - 1)) / 2;

        if (rest == p2)                 /* matches the required form */
            return pseq;
    }

    /* any remaining factor > sqrt(n) is prime */
    if (temp > 1) {
        idx_t p = temp;                 /* remaining prime factor */
        /* exponent e = 1 automatically */
        seq_t pseq = (seq_t)p;
        seq_t p_pow = pseq;             /* p^1 */
        seq_t rest = (seq_t)orig / p_pow;
        seq_t p2 = (p == 2) ? (seq_t)1 : ((seq_t)(p - 1)) / 2;
        if (rest == p2)
            return pseq;
    }

    return (seq_t)1;           /* no prime satisfies the condition */
}
