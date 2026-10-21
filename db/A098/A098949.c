/*
 * A098949  Numbers where 9 is the only odd decimal digit.
 * https://oeis.org/A098949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mul_idx, pow_idx, precompute_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n-th number whose decimal digits are drawn from
   {0,2,4,6,8,9} and that contains at least one 9, in increasing order.
   The term a(1) = 9.  For n = 0 the function returns 0. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_idx.h"
#include "../utils/pow_idx.h"
#include "../utils/precompute_powers.h"

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* a(1) = 9, so return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Find the digit length d such that the n-th term lies in the
       block of d‑digit numbers. */
    idx_t cum = 0;          /* cumulative count of terms with fewer than d digits */
    idx_t d = 0;            /* desired digit length */
    idx_t max_digits = 0;   /* upper bound on the number of digits needed */
    /* Estimate an upper bound: 6^d grows fast; for n up to 2^63
       d <= 20 is safe. */
    for (max_digits = 1; ; ++max_digits)
    {
        idx_t pow6d_1 = pow_idx((idx_t)6, max_digits - 1);
        idx_t pow5d_1 = pow_idx((idx_t)5, max_digits - 1);
        idx_t cnt_d = 5 * pow6d_1 - 4 * pow5d_1;   /* 5*6^(d-1) - 4*5^(d-1) */
        if (cum + cnt_d >= n)
        {
            d = max_digits;
            break;
        }
        cum += cnt_d;
    }

    /* Offset within the d‑digit block (0‑based). */
    idx_t offset = n - cum - 1;  /* 0 <= offset < count_of_d_digits */

    /* Precompute powers of 5 and 6 up to d. */
    idx_t pow5[d + 1];
    idx_t pow6[d + 1];
    precompute_powers(d, pow5, pow6);

    /* Allowed digits in ascending order. */
    const int first_digits[]  = {2, 4, 6, 8, 9};          /* first position (no 0) */
    const int other_digits[]  = {0, 2, 4, 6, 8, 9};

    bool has9 = false;       /* whether a 9 has already appeared in the prefix */
    seq_t value = 0;         /* accumulated value */

    for (idx_t pos = 0; pos < d; ++pos)
    {
        int *digit_set;
        int  set_size;
        if (pos == 0)
        {
            digit_set = (int *)first_digits;
            set_size  = sizeof(first_digits) / sizeof(first_digits[0]);
        }
        else
        {
            digit_set = (int *)other_digits;
            set_size  = sizeof(other_digits) / sizeof(other_digits[0]);
        }

        idx_t rem = d - 1 - pos;   /* remaining positions after this digit */

        for (int j = 0; j < set_size; ++j)
        {
            int digit = digit_set[j];
            bool new_has9 = has9 || (digit == 9);
            idx_t cnt;

            if (rem == 0)
            {
                cnt = new_has9 ? 1 : 0;
            }
            else if (new_has9)
            {
                cnt = pow6[rem];
            }
            else /* new_has9 == false */
            {
                /* suffixes of length rem that contain at least one 9:
                   6^rem - 5^rem */
                cnt = pow6[rem] - pow5[rem];
            }

            if (offset < cnt)
            {
                /* select this digit */
                value = (seq_t)((seq_t)value * (seq_t)10 + (seq_t)digit);
                has9   = new_has9;
                break;
            }
            else
            {
                offset -= cnt;
            }
        }
    }

    return value;
}
