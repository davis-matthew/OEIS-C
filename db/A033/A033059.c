/*
 * A033059  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mul_saturate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers in base‑8 whose digit in every position j with (L‑1 + j) odd is odd
 * (L = number of digits, j = 0 is the least significant digit).
 * The function returns the nth such number (n ≥ 1).  The sequence starts
 * with 1, 2, 3, 4, 5, 6, 7, 9, 11, … as given in the OEIS entry A033059.*/

#include <stdint.h>
#include "../utils/mul_saturate.h"

seq_t a(idx_t n)
{
    if (n <= 7)            /* first run of 1–7 */
        return (seq_t)n;

    /* --- find the length L of the number that contains the nth term --- */
    uint64_t cumulative = 0;   /* terms counted so far */
    uint64_t count = 0;        /* terms for current length L */
    int L = 1;                 /* current length */

    for (;; ++L)
    {
        int required = L / 2;              /* number of required odd digits */
        int free = L - required;           /* number of free positions         */
        /* free positions: one (j = L-1) is the leading digit, others are any 0–7 */
        count = 7;                         /* leading digit options             */
        for (int k = 0; k < required; ++k)
            count = mul_saturate(count, 4);
        for (int k = 0; k < free - 1; ++k)
            count = mul_saturate(count, 8);

        if (cumulative + count >= n)
            break;
        cumulative = mul_saturate(cumulative, 1) + count;  /* safe add */
    }

    uint64_t offset = n - cumulative - 1;   /* 0‑based index within length L */

    /* --- prepare option counts and suffix products for positions j = 0..L-1 --- */
    int m = L - 1;                          /* highest index (most significant) */
    uint8_t  options[65];
    uint64_t suffix[65];                    /* suffix[j] = product of options for j-1 .. 0 */
    suffix[0] = 1;

    for (int j = 0; j <= m; ++j)
    {
        int requiredOdd = (((L - 1) + j) & 1);
        if (requiredOdd)
            options[j] = 4;                         /* only odd digits 1,3,5,7 */
        else
            options[j] = (j == m) ? 7 : 8;          /* leading: 1–7, else: 0–7 */

        if (j > 0)
            suffix[j] = mul_saturate(suffix[j - 1], options[j - 1]);
    }

    /* --- construct the number digit by digit (from most significant to least) --- */
    seq_t value = 0;
    for (int j = m; j >= 0; --j)
    {
        value <<= 3;   /* multiply by 8 for the next digit */

        int first_digit = 0;
        int last_digit  = 7;
        if (((L - 1) + j) & 1)
        {   /* required odd digit */
            first_digit = 1; last_digit = 7;
        }
        else if (j == m)
        {   /* leading free digit (non‑zero) */
            first_digit = 1;
        }

        for (int d = first_digit; d <= last_digit; d += ((((L - 1) + j) & 1) ? 2 : 1))
        {
            uint64_t comb = suffix[j];
            if (offset >= comb)
                offset -= comb;
            else
            {
                value += (seq_t)d;
                break;
            }
        }
    }

    return value;
}
