/*
 * A084637  Binomial transform of (1,0,1,0,1,0,1,1,1,1,1,...).
 * https://oeis.org/A084637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - (C(n,1) + C(n,3) + C(n,5))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a 128‑bit intermediate for the small binomial terms. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
    }

    /* Compute the correction term C(n,1)+C(n,3)+C(n,5) */
    seq_t corr = (seq_t)n;  /* C(n,1) */

    if (n >= 3) {
        unsigned __int128 prod = (unsigned __int128)n * (n - 1) * (n - 2);
        prod /= 6;           /* divide by 3! */
        corr += (seq_t)prod; /* add C(n,3) */
    }

    if (n >= 5) {
        unsigned __int128 prod = (unsigned __int128)n * (n - 1) * (n - 2) *
                                 (n - 3) * (n - 4);
        prod /= 120;         /* divide by 5! */
        corr += (seq_t)prod; /* add C(n,5) */
    }

    return pow2 - corr;
}
