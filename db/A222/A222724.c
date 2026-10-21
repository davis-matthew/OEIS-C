/*
 * A222724  Palindromic nonprime numbers starting with a digit 1.
 * https://oeis.org/A222724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 273, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindromic nonprime number that starts with the digit 1.
   The index is 1‑based (a(1) = 1). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Count of palindromes found so far */
    idx_t count = 0;

    /* Generate palindromes by increasing length */
    for (idx_t len = 1; ; ++len)
    {
        /* half = ceil(len/2) – number of digits in the left half (including the middle digit if odd) */
        idx_t half = (len + 1) / 2;

        seq_t prefix_start = pow10(half - 1);   /* first digit fixed to 1 */
        seq_t prefix_end   = prefix_start * 2 - 1;   /* only prefixes that begin with 1 */

        seq_t pow10_half   = pow10(half);
        seq_t pow10_halfm1 = pow10(half - 1);

        if (len & 1) /* odd length */
        {
            for (seq_t prefix = prefix_start; prefix <= prefix_end; ++prefix)
            {
                seq_t pal = prefix * pow10_halfm1 + reverse(prefix / 10);
                if (!is_prime_big(pal))
                {
                    ++count;
                    if (count == n) return pal;
                }
            }
        }
        else /* even length */
        {
            for (seq_t prefix = prefix_start; prefix <= prefix_end; ++prefix)
            {
                seq_t pal = prefix * pow10_half + reverse(prefix);
                if (!is_prime_big(pal))
                {
                    ++count;
                    if (count == n) return pal;
                }
            }
        }
    }

    /* Should never reach here for valid n */
    return 0;
}
