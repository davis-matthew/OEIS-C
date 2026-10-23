/*
 * A132189  Number of non-constant 3-term geometric progressions with no term
 *          exceeding n.
 * https://oeis.org/A132189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{c=2}^{⌊√n⌋} φ(c) * ⌊n / c²⌋, where φ is Euler's
 *            totient. This counts each 3‑term geometric progression twice,
 *            once for (b,c) and once for (c,b)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Integer square‑root for 64‑bit unsigned values */
static idx_t isqrt_u64(idx_t n)
{
    /* Start with a floating point approximation */
    idx_t x = (idx_t)floor(sqrtl((long double)n));
    /* Correct any off‑by‑one errors */
    while ((x + 1) <= n / (x + 1)) ++x;
    while (x > n / x) --x;
    return x;
}

/* Euler's totient φ(k) for k ≥ 1, using 64‑bit trial division */
static seq_t phi_u64(idx_t k)
{
    seq_t result = (seq_t)k;
    idx_t m = k;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            result = result - result / (seq_t)p;
        }
    }
    if (m > 1) result = result - result / (seq_t)m;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* all terms are zero for n = 1,2,3 */

    idx_t max_c = isqrt_u64(n);
    seq_t sum = 0;

    for (idx_t c = 2; c <= max_c; ++c)
    {
        seq_t terms = (seq_t)n / ((seq_t)c * (seq_t)c);
        seq_t ph   = phi_u64(c);
        sum += ph * terms;
    }

    return sum * (seq_t)2;
}
