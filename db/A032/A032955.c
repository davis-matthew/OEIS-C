/*
 * A032955  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer whose base‑5 representation
   has even digits at all odd positions (positions counted from the
   most significant digit, starting at 0).
   The sequence begins: 1, 2, 3, 4, 5, 7, 9, 10, 12, ... */

#include <stdint.h>

static seq_t mul_cap(seq_t a, int m, seq_t limit)
{
    seq_t r = 0;
    for (int i = 0; i < m; ++i)
    {
        r += a;
        if (r > limit)
            return limit;          /* cap at limit (the largest useful value) */
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Target index, converted to seq_t for arithmetic */
    seq_t target = (seq_t)n;

    /* First determine the number of base‑5 digits of the desired number. */
    idx_t len = 0;
    seq_t cum = 0;              /* cumulative count of numbers with fewer digits */

    for (;;)
    {
        ++len;
        seq_t count = 1;

        for (idx_t pos = 0; pos < len; ++pos)
        {
            int m;
            if (pos == 0)
                m = 4;          /* digits 1..4 */
            else if (pos & 1)
                m = 3;          /* even digits 0,2,4 */
            else
                m = 5;          /* digits 0..4 */

            count = mul_cap(count, m, target);
        }

        /* If cum + count >= target, the desired number has 'len' digits. */
        /* We avoid overflow by comparing count with (target - cum). */
        if (count >= target - cum)
            break;
        cum += count;
    }

    /* Offset within the block of numbers with 'len' digits (zero‑based). */
    seq_t rem = target - cum - 1;

    /* Allowed digit counts per position; small integers. */
    int allowed[512];   /* maximum length reasonable for n up to 10000 */
    for (idx_t pos = 0; pos < len; ++pos)
    {
        if (pos == 0)
            allowed[pos] = 4;
        else if (pos & 1)
            allowed[pos] = 3;
        else
            allowed[pos] = 5;
    }

    /* Suffix products: number of completions for positions > pos. */
    seq_t suffix[513];
    suffix[len] = 1;
    for (int pos = (int)len - 1; pos >= 0; --pos)
        suffix[pos] = mul_cap(suffix[pos + 1], allowed[pos], target);

    /* Build the digits of the desired number. */
    int digit[512];
    for (idx_t pos = 0; pos < len; ++pos)
    {
        seq_t step = suffix[pos + 1];

        if (pos == 0)
        {
            for (int d = 1; d <= 4; ++d)
            {
                if (rem < step)
                {
                    digit[pos] = d;
                    break;
                }
                rem -= step;
            }
        }
        else if (pos & 1)
        {
            static const int evens[3] = {0, 2, 4};
            for (int k = 0; k < 3; ++k)
            {
                if (rem < step)
                {
                    digit[pos] = evens[k];
                    break;
                }
                rem -= step;
            }
        }
        else
        {
            for (int d = 0; d <= 4; ++d)
            {
                if (rem < step)
                {
                    digit[pos] = d;
                    break;
                }
                rem -= step;
            }
        }
    }

    /* Convert the digit array into the final base‑5 number. */
    seq_t result = 0;
    for (idx_t pos = 0; pos < len; ++pos)
        result = result * 5 + (seq_t)digit[pos];

    return result;
}
