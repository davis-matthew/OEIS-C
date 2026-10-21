/*
 * A092342  a(n) = sigma_3(3n+1).
 * https://oeis.org/A092342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ₃(3n + 1) (sum of cubes of all divisors of 3n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation of a seq_t base to an idx_t exponent.
   Works for any seq_t that supports multiplication, addition, subtraction. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp != 0)
    {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* m = 3n + 1 ------------------------------------- */
    uint64_t m = 3 * n + 1u;
    seq_t sigma = 1;                 /* product over factor blocks            */

    /* factor m by trial division --------------------------------- */
    uint64_t temp = m;
    /* handle factor 2 */
    if ((temp & 1u) == 0u)          /* temp is even */
    {
        idx_t e = 0;
        while ((temp & 1u) == 0u) { temp >>= 1; ++e; }
        /* compute σ₃ for 2ᵉ */
        seq_t p   = 2;
        seq_t num = pow_seq_t(p, 3 * (e + 1)) - 1;
        seq_t den = pow_seq_t(p, 3) - 1;
        sigma *= num / den;
    }

    /* handle odd factors ------------------------------------------ */
    for (uint64_t d = 3; d * d <= temp; d += 2)
    {
        if (temp % d == 0u)
        {
            idx_t e = 0;
            while (temp % d == 0u) { temp /= d; ++e; }

            seq_t p   = (seq_t)d;
            seq_t num = pow_seq_t(p, 3 * (e + 1)) - 1;
            seq_t den = pow_seq_t(p, 3) - 1;
            sigma *= num / den;
        }
    }

    /* remaining prime factor (if any) -------------------------------- */
    if (temp > 1u)
    {
        seq_t p   = (seq_t)temp;
        seq_t num = pow_seq_t(p, 3 * (1 + 1)) - 1;  /* e = 1 */
        seq_t den = pow_seq_t(p, 3) - 1;
        sigma *= num / den;
    }

    return sigma;
}
