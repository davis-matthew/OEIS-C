/*
 * A065144  Smallest prime containing the n-th square in decimal notation.
 * https://oeis.org/A065144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime that contains the decimal representation
 *            of n² as a contiguous substring
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 33, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* square = n² */
    seq_t square = (seq_t)n * (seq_t)n;

    /* convert square to decimal string */
    char square_buf[256];
    size_t square_len;
    seq_to_str(square, square_buf, &square_len);

    seq_t candidate = 2;                    /* start with the first prime */
    for (;;)
    {
        if (is_prime_big(candidate))
        {
            /* convert current prime to decimal string */
            char prime_buf[256];
            size_t prime_len;
            seq_to_str(candidate, prime_buf, &prime_len);

            /* check if square digits appear as a contiguous substring */
            if (prime_len >= square_len)
            {
                for (size_t pos = 0; pos + square_len <= prime_len; ++pos)
                {
                    if (memcmp(&prime_buf[pos], square_buf, square_len) == 0)
                        return candidate;
                }
            }
        }

        /* advance to next candidate prime (skip even numbers after 2) */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}
