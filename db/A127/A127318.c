/*
 * A127318  a(n) = binomial(n, sd) mod sd, with sd = sum of decimal digits of
 *          n.
 * https://oeis.org/A127318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = binomial(n, sd) mod sd, where sd = sum of decimal digits
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_gcd_idx.h"

/* Compute greatest common divisor for unsigned 64‑bit integers */

seq_t a(idx_t n)
{
    /* compute digit sum */
    idx_t sum = 0;
    for (idx_t t = n; t > 0; t /= 10) sum += t % 10;

    seq_t mod = (seq_t)sum;
    if (mod == 1) return 0;

    idx_t k = sum;        /* k = sd */
    if (k > n) return 0;  /* not expected */

    /* array of numerators: n-k+1 .. n */
    /* maximum k for 64‑bit n is 9*20 = 180, so array size 200 is safe */
    static idx_t nums[200];
    for (idx_t i = 0; i < k; ++i)
        nums[i] = n - k + 1 + i;

    /* cancel denominators */
    for (idx_t d = 1; d <= k; ++d)
    {
        idx_t rem = d;
        for (idx_t j = 0; j < k && rem > 1; ++j)
        {
            idx_t g = _gcd_idx(nums[j], rem);
            if (g > 1)
            {
                nums[j] /= g;
                rem    /= g;
            }
        }
        /* rem should be 1 here for an exact integer binomial */
    }

    /* compute product modulo mod */
    seq_t prod = 1 % mod;
    idx_t mod_idx = (idx_t)mod;          /* for modulo in idx_t domain */

    for (idx_t i = 0; i < k; ++i)
    {
        seq_t val = (seq_t)(nums[i] % mod_idx);
        unsigned long long a1 = (unsigned long long)prod;
        unsigned long long b1 = (unsigned long long)val;
        unsigned long long m1 = (unsigned long long)mod;
        prod = (seq_t)((a1 * b1) % m1);
    }

    return prod;
}
