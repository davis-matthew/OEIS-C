/*
 * A038370  Smallest prime with "n^2" as central digit(s).
 * https://oeis.org/A038370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest prime whose decimal representation has
   'n^2' as the centered block of digits.  For a given n
   we search over increasing widths k of the equal
   number of digits on both sides, generating all
   numbers left * 10^(m+k) + n^2 * 10^k + right,
   where m is the number of decimal digits of n^2,
   left ranges over 10^(k-1)..10^k-1 and
   right ranges over 0..10^k-1.  The first prime found
   for a given k is returned. */

#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;                 /* n^2          */
    idx_t   m  = (sq == 0) ? 1 : digits((idx_t)sq); /* digits of n^2 */

    /* search over increasing side width k */
    for (idx_t k = 1; ; ++k)
    {
        seq_t pow10_k   = pow10(k);           /* 10^k          */
        seq_t pow10_k1  = pow10(k-1);         /* 10^(k-1)      */
        seq_t mult      = pow10(m + k);       /* 10^(m+k)      */
        seq_t mid       = sq * pow10_k;       /* n^2 * 10^k    */

        idx_t left_start = (idx_t)pow10_k1;
        idx_t left_end   = (idx_t)(pow10_k - 1);

        for (idx_t left = left_start; left <= left_end; ++left)
        {
            seq_t base = (seq_t)left * mult + mid;
            for (idx_t right = 0; right < (idx_t)pow10_k; ++right)
            {
                seq_t candidate = base + (seq_t)right;
                if (is_prime_big(candidate))
                    return candidate;
            }
        }
    }
}
