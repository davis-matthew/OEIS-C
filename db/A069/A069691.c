/*
 * A069691  Smallest prime with internal digits = n; or 0 if no such number
 *          exists.
 * https://oeis.org/A069691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Smallest prime formed by placing one digit on each side of the
 *            decimal representation of n. Returns 0 if no such prime exists
 * Range:     u8 none, u16 n <= 626, u32 n <= 1989, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, is_prime_big, mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* number of digits of n (0 is treated as 1 digit). */
    int len = (n == 0) ? 1 : digits(n);

    seq_t pow10_len1 = pow10((idx_t)len + 1);          /* 10^(len+1)   */
    seq_t n_times_10 = mul_small((seq_t)n, 10);       /* n * 10       */

    static const unsigned right[4] = {1, 3, 7, 9};

    seq_t best = 0;
    bool found = false;

    for (unsigned left = 1; left <= 9; ++left)
    {
        seq_t left_part = mul_small(pow10_len1, left);   /* left * 10^(len+1) */
        for (int i = 0; i < 4; ++i)
        {
            seq_t cand = left_part + n_times_10 + (seq_t)right[i];
            if (is_prime_big(cand))
            {
                if (!found || cand < best)
                {
                    best = cand;
                    found = true;
                }
            }
        }
    }
    return found ? best : 0;
}
