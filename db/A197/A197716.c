/*
 * A197716  a(n) = ceiling((2n+1/n)^n).
 * https://oeis.org/A197716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ceil((2*n + 1/n)^n) = ceil(((2*n^2+1)/n)^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case of n == 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* Small integral seq_t (uint8_t … uint64_t, unsigned __int128):
       Perform the calculation with 128‑bit intermediates to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        /* n fits in 64 bits, so 2*n*n+1 fits in 128 bits for the allowed values */
        __int128 n128   = (unsigned long long)n;
        __int128 base   = 2 * n128 * n128 + 1;   /* (2n^2 + 1) */
        __int128 num    = 1;
        __int128 den    = 1;

        /* Compute base^n and n^n */
        for (idx_t i = 0; i < n; ++i) {
            num *= base;
            den *= (idx_t)n;
        }

        /* Ceil division: q = num/den, r = num%den */
        __int128 q = num / den;
        __int128 r = num % den;

        __int128 ans128 = q + (r > 0 ? 1 : 0);
        return (seq_t)ans128;
    }

    /* Arbitrary‑precision seq_t: use the big_pow helper */
    seq_t n_seq = (unsigned long long)n;          /* construct from uint64_t */
    seq_t base  = 2 * n_seq * n_seq + 1;          /* (2n^2 + 1) as seq_t */

    seq_t num  = big_pow(base, n_seq);            /* (2n^2+1)^n */
    seq_t den  = big_pow(n_seq, n_seq);          /* n^n             */
    seq_t q    = num / den;                      /* floor result    */
    seq_t r    = num % den;                      /* remainder       */

    seq_t ans  = q;
    if (r > 0)
        ans = ans + 1;

    return ans;
}
