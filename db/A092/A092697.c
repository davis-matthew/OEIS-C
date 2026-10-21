/*
 * A092697  For 1 <= n <= 9, a(n) = least number m such that the product n*m
 *          is obtained merely by shifting the rightmost digit of m to the
 *          left end (a finite sequence).
 * https://oeis.org/A092697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   a(n) = min_{k=n..9}
 *            (k*(10^(ord(10,(10n-1)/gcd(k,10n-1)))-1)/(10n-1))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for 1 <= n <= 9; otherwise return 0. */
    if (n < 1 || n > 9)
        return (seq_t)0;

    seq_t denom = (seq_t)(10 * n - 1);   /* constant (10n‑1) */
    seq_t best  = (seq_t)0;
    bool found  = false;

    for (idx_t k = n; k <= 9; ++k)
    {
        seq_t kSeq = (seq_t)k;
        seq_t g    = gcd(kSeq, denom);
        seq_t t    = denom / g;          /* t = (10n-1)/gcd(k,10n-1) */
        uint64_t d = (uint64_t)t;

        uint64_t order;
        if (d == 1)
        {
            /* When t == 1 the multiplicative order is defined as 1. */
            order = 1;
        }
        else
        {
            order = 1;
            uint64_t cur = 10 % d;
            while (cur != 1)
            {
                cur = (cur * 10) % d;
                ++order;
            }
        }

        /* Compute (10^order - 1) */
        seq_t pow_term = pow10(order);
        pow_term -= (seq_t)1;

        /* m = k * (10^order - 1) / (10n-1) */
        seq_t m = kSeq * pow_term;
        m = m / denom;

        if (!found || m < best)
        {
            best  = m;
            found = true;
        }
    }

    return best;
}
