/*
 * A214312  a(n) is the number of all four-color bracelets (necklaces with
 *          turning over allowed) with n beads and the four colors are from a
 *          repertoire of n distinct colors, for n >= 4.
 * https://oeis.org/A214312
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,4) * exact_four_from_set(n) where
 *            exact_four_from_set(n) is the number of 4‑coloured bracelets of
 *            length n that use all four colours
 * Range:     u8 none, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   big_pow, binom_n_4, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"
#include "../utils/binom_n_4.h"

/* Small integer totient for d <= 10000 (fits in uint64_t). */
static seq_t phi_uint64(idx_t d)
{
    idx_t result = d;
    for (idx_t p = 2; p * p <= d; ++p)
    {
        if (d % p == 0)
        {
            while (d % p == 0) d /= p;
            result -= result / p;
        }
    }
    if (d > 1) result -= result / d;
    return (seq_t)result;           /* fits in seq_t safely */
}

/* Number of bracelets of length n over an alphabet of size k,
   allowing unused colours.   */
static seq_t bracelet_count(idx_t n, idx_t k)
{
    if (k == 0) return 0;

    seq_t rot_sum = 0;
    idx_t divs[128];
    size_t divcnt = 0;

    /* collect all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            divs[divcnt++] = d;
            if (d != n / d) divs[divcnt++] = n / d;
        }
    }

    /* rotations */
    for (size_t j = 0; j < divcnt; ++j)
    {
        idx_t d = divs[j];
        idx_t exp = n / d;
        seq_t pow_val = big_pow((seq_t)k, (seq_t)exp);
        seq_t phi_d   = phi_uint64(d);
        rot_sum += mul_small(pow_val, (unsigned int)phi_d);
    }

    /* reflections */
    seq_t refl_sum;
    if (n & 1)   /* n odd */
    {
        seq_t pow_val = big_pow((seq_t)k, (seq_t)((n + 1) / 2));
        refl_sum = mul_small(pow_val, (unsigned int)n);
    }
    else         /* n even */
    {
        seq_t pow1 = big_pow((seq_t)k, (seq_t)(n / 2 + 1));
        seq_t pow2 = big_pow((seq_t)k, (seq_t)(n / 2));
        seq_t part1 = mul_small(pow1, (unsigned int)(n / 2));
        seq_t part2 = mul_small(pow2, (unsigned int)(n / 2));
        refl_sum = part1 + part2;
    }

    seq_t total = rot_sum + refl_sum;
    seq_t denom = (seq_t)(2 * n);
    return total / denom;
}

/* Number of 4‑coloured bracelets of length n that use all four colours. */
static seq_t exact_four_from_set(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    const unsigned comb[5] = {1, 4, 6, 4, 1};

    for (int i = 0; i <= 4; ++i)
    {
        seq_t term = bracelet_count(n, 4 - i);
        term = mul_small(term, comb[i]);

        if ((i & 1) == 0) pos += term;
        else              neg += term;
    }
    return pos - neg;   /* pos >= neg for n >= 4 */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t bin = binom_n_4(n);
    seq_t four = exact_four_from_set(n);
    return bin * four;
}
