/*
 * A100472  Inverse modulo 2 modulo transform of 9^n.
 * https://oeis.org/A100472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*(binomial(n,k) mod 2)*9^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* precompute powers of 9 up to n */
    seq_t *pow9 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow9) return (seq_t)0;                 /* allocation failure → zero */
    pow9[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow9[i] = pow9[i - 1] * (seq_t)9;

    seq_t pos = (seq_t)0;   /* sum of terms with positive sign   */
    seq_t neg = (seq_t)0;   /* sum of terms with negative sign  */

    /* enumerate all k ⊆ n (binomial(n,k) mod 2 = 1) */
    idx_t mask = n;
    do {
        idx_t k   = mask;
        idx_t nk  = n - k;                          /* n - k  */
        unsigned int pc = (unsigned int)__builtin_popcountll(nk);
        if (pc & 1)                                 /* odd popcount → negative sign */
            neg += pow9[(size_t)k];
        else                                         /* even popcount → positive sign */
            pos += pow9[(size_t)k];

        mask = (mask - 1) & n;                      /* next subset of n */
    } while (mask != n);

    free(pow9);
    return pos - neg;
}
