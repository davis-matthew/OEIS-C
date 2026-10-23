/*
 * A028984  Palindromes whose sum of divisors is odd.
 * https://oeis.org/A028984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   floor_sqrt, rev_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromes whose sum of divisors is odd
 * (a palindrome is included iff it is a perfect square
 * or twice a perfect square). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/rev_int.h"

/* Return the nth element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1; return 0 as a safe value */

    idx_t count = 0;          /* how many valid palindromes found so far */

    for (int len = 1; ; ++len) {
        bool is_even = (len % 2 == 0);
        idx_t half_len = (idx_t)((len + 1) / 2);   /* number of digits in the prefix */

        /* compute 10^(half_len-1) and 10^half_len */
        seq_t pow10_half = 1;
        for (int i = 0; i < half_len; ++i)
            pow10_half *= 10;
        seq_t pow10_half_minus1 = pow10_half / 10;
        seq_t start = pow10_half_minus1;   /* first prefix with the required length */
        seq_t end   = pow10_half - 1;      /* last prefix with the required length */

        for (seq_t pref = start; pref <= end; ++pref) {
            seq_t pref_s  = pref;
            seq_t rev_pref = rev_int(is_even ? pref_s : pref_s / 10);
            seq_t pal = pref_s * (is_even ? pow10_half : pow10_half_minus1) + rev_pref;

            if (pal == 1)
                continue;   /* 1 is not part of the sequence */

            seq_t sq = floor_sqrt(pal);
            bool good = (sq * sq == pal);
            if (!good && (pal % 2 == 0)) {
                seq_t half = pal / 2;
                seq_t sq_half = floor_sqrt(half);
                if (sq_half * sq_half == half)
                    good = true;
            }

            if (good) {
                ++count;
                if (count == n)
                    return pal;
            }
        }
    }
}
