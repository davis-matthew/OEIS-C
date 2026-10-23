/*
 * A129764  Numerator of the sum of all elements of n X n X n cubic array
 *          M[i,j,k] = 1/(i+j+k-2).
 * https://oeis.org/A129764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Numerator[ Sum_{i=1}^n Sum_{j=1}^n Sum_{k=1}^n
 *            1/(i+j+k-2) ]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   binom_choose_2, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/binom_choose_2.h"

/* Count triples 1 ≤ i,j,k ≤ n with i + j + k = t + 2.
   Uses inclusion–exclusion with binomial coefficients about n. */
static inline seq_t count_triples(idx_t t, idx_t n)
{
    seq_t res = 0;

    /* Term 1: C(t+1,2) */
    res += binom_choose_2((seq_t)(t + 1));

    /* Term 2: -3*C(t - n + 1,2)  for t ≥ n+1 */
    if (t >= n + 1)
        res -= 3 * binom_choose_2((seq_t)(t - n + 1));

    /* Term 3: +3*C(t - 2n + 1,2)  for t ≥ 2n+1 */
    if (t >= 2 * n + 1)
        res += 3 * binom_choose_2((seq_t)(t - 2 * n + 1));

    /* Term 4: -C(t - 3n + 1,2)  for t ≥ 3n+1 */
    if (t >= 3 * n + 1)
        res -= binom_choose_2((seq_t)(t - 3 * n + 1));

    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety check */

    seq_t num = 0;   /* numerator of accumulated sum */
    seq_t den = 1;   /* denominator of accumulated sum */

    idx_t T = 3 * n - 2;   /* largest t = i+j+k-2 */

    for (idx_t t = 1; t <= T; ++t)
    {
        seq_t f = count_triples(t, n);   /* numerator of current term */
        seq_t term_num = f;
        seq_t term_den = (seq_t)t;

        /* new_num = num * term_den + term_num * den          */
        /* new_den = den * term_den                           */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;

        /* reduce by gcd to keep values small */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator after full reduction */
}
