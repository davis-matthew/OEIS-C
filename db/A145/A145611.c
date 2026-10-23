/*
 * A145611  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=2.
 * https://oeis.org/A145611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{d=1}^{2n} 2^(2n+1-d) / d (index l =
 *            2n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* GCD for unsigned __int128 (needed when seq_t is 64‑bit) */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != (unsigned __int128)0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* l = 2n + 1 (odd) */
    idx_t l = ((idx_t)((seq_t)n * 2) + (idx_t)1);

    /* Base case: l = 3 -> a(1) = 5 */
    if (l == (idx_t)3) {
        return (seq_t)5;
    }

    /* Start from l = 3 with A_3 = 5/1 */
    seq_t N = (seq_t)5;    /* numerator of A_l(2) */
    seq_t D = (seq_t)1;    /* denominator of A_l(2) */
    idx_t cur = (idx_t)3;  /* current l */

    while (cur < l) {
        /* Compute A_{cur+1} = 2*A_cur + 2/cur */
        unsigned __int128 Ni128 = ((unsigned __int128)N * (unsigned __int128)cur * (unsigned __int128)2)
                                 + ((unsigned __int128)D * (unsigned __int128)2);
        unsigned __int128 Di128 = ((unsigned __int128)D * (unsigned __int128)cur);

        /* Reduce the fraction */
        unsigned __int128 g128 = gcd128(Ni128, Di128);
        Ni128 /= g128;
        Di128 /= g128;

        /* Cast back to seq_t (fits because we reduced) */
        N = (seq_t)Ni128;
        D = (seq_t)Di128;

        ++cur;
    }

    return N;
}
