/*
 * A118266  Coefficient of q^n in (1-q)^5/(1-5q); dimensions of the enveloping
 *          algebra of the derived free Lie algebra on 5 letters.
 * https://oeis.org/A118266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{min(n,5)} (-1)^k * C(5,k) * 5^(n‑k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = 1024 * 5^(n‑5)   for n > 5  */

#include <stdint.h>

/* fast exponentiation: base^exp, where exp is idx_t (>=0) */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* binomial coefficients C(5,k) for k=0..5 */
    static const unsigned int binom5[6] = {1, 5, 10, 10, 5, 1};

    if (n <= 5) {
        seq_t evenSum = 0;
        seq_t oddSum  = 0;
        for (idx_t k = 0; k <= n; ++k) {
            seq_t pow   = pow_uint((seq_t)5, n - k);
            seq_t term  = pow * (seq_t)binom5[k];
            if (k & 1)
                oddSum += term;
            else
                evenSum += term;
        }
        return evenSum - oddSum;          /* evenSum ≥ oddSum for n≤5 */
    } else {
        seq_t pow = pow_uint((seq_t)5, n - 5);
        return (seq_t)1024 * pow;         /* 1024 = C(5,0)*5^5 */
    }
}
