/*
 * A214938  Number of Motzkin n-paths avoiding even-numbered steps that are
 *          flat steps.
 * https://oeis.org/A214938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(⌊(n+1)/2⌋, (n mod 2) + 2·(⌊n/4⌋
 *            – k)) * catalan(k + ⌊(n+2)/4⌋) Uses helpers: binomial(idx_t a,
 *            idx_t b) and catalan(idx_t k) from utils
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 56, u128 n <= 106,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Frequently used sub‑expressions */
    idx_t m      = (n + 1) >> 1;      /* ⌊(n+1)/2⌋ */
    idx_t t      = n >> 2;            /* ⌊n/4⌋ */
    idx_t off    = (n + 2) >> 2;      /* ⌊(n+2)/4⌋ */
    idx_t parity = n & 1;             /* n mod 2 */

    seq_t sum = 0;

    for (idx_t k = 0; k <= t; ++k)
    {
        idx_t kk = parity + 2 * (t - k);   /* upper index for binomial */
        if (kk > m)                       /* impossible binomial */
            continue;

        seq_t bin = binomial(m, kk);
        seq_t cat = catalan(k + off);
        sum += bin * cat;
    }

    return sum;
}
