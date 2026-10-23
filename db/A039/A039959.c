/*
 * A039959  Number of ways of numbering the vertices of a cube so sum of the 8
 *          numbers is n.
 * https://oeis.org/A039959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/24)*C(n+7,7) + (3/8)*C(⌊n/2⌋+3,3) +
 *            (1/3)*∑_{j=0}^{⌊n/3⌋}(n-3j+1)(j+1) + (1/4)*[n≡0 (mod 4)](n/4+1)
 *            (C(k,m)=binomial(k,m))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 57, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute C(n,k) where k≤7 (uses only integer arithmetic) */
static seq_t binom_small(idx_t n, unsigned k)
{
    if (k > n) return 0;
    seq_t r = 1;
    for (unsigned i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Term 1: (1/24)*C(n+7,7) */
    seq_t term1 = binom_small(n + 7u, 7u);

    /* Term 2: (3/8)*C(⌊n/2⌋+3,3)  (n even only) */
    seq_t term2 = 0;
    if ((n & 1u) == 0u) {
        idx_t m2 = n >> 1;                     /* m2 = ⌊n/2⌋ */
        term2 = binom_small(m2 + 3u, 3u);
    }

    /* Term 3: (1/3)*∑_{j=0}^{⌊n/3⌋}(n-3j+1)(j+1)  */
    idx_t m3 = n / 3u;                          /* ⌊n/3⌋ */
    seq_t S = ((seq_t)(n + 1u) * (seq_t)(m3 + 1u) * (seq_t)(m3 + 2u)) / (seq_t)2u;
    seq_t temp = (seq_t)m3 * (seq_t)(m3 + 1u) * (seq_t)(m3 + 2u);
    S = S - temp;                               /* S = coefficient of convolution */

    /* Term 4: (1/4)*(n/4+1) if n divisible by 4 */
    seq_t term4 = 0;
    if ((n & 3u) == 0u) {
        idx_t m4 = n / 4u;                      /* n/4 */
        term4 = (seq_t)(m4 + 1u);
    }

    /* Combine with common denominator 24:
         a(n) = [C1 + 9*C2 + 8*S + 6*term4] / 24           */
    seq_t numerator = term1
                     + (seq_t)9u * term2
                     + (seq_t)8u * S
                     + (seq_t)6u * term4;

    return numerator / (seq_t)24u;
}
