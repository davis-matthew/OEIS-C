/*
 * A033668  Theta series of 4-dimensional lattice A_2 tensor A2, with det 81,
 *          minimal norm 4.
 * https://oeis.org/A033668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 0 if n ≡ 1 (mod 3) a(n) = 6·σ(n) if n ≡ 2 (mod
 *            3) For n ≡ 0 (mod 3): let k = exponent of 3 in n if k = 1 → a(n)
 *            = 3·σ(n) if k ≥ 2 → a(n) = 12·σ(n / 3^k) (i.e. σ of n with all
 *            factors of 3 removed)
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t sigma_u64(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; (idx_t)(d * d) <= n; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)d;
            idx_t q = n / d;
            if (q != d) sum += (seq_t)q;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t mod3 = n % 3;
    if (mod3 == 1) return (seq_t)0;

    if (mod3 == 2) /* n ≡ 2 (mod 3) */
    {
        seq_t s = sigma_u64(n);
        return (seq_t)6 * s;
    }

    /* n ≡ 0 (mod 3) */
    idx_t t = n;
    int k = 0;
    while ((t % 3) == 0)
    {
        t /= 3;
        ++k;
    }

    if (k == 1) /* exactly one factor of 3 */
    {
        seq_t s = sigma_u64(n);
        return (seq_t)3 * s;
    }
    else /* k ≥ 2 : n divisible by 9 */
    {
        seq_t s = sigma_u64(t); /* t = n / 3^k */
        return (seq_t)12 * s;
    }
}
