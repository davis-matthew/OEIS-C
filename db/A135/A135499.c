/*
 * A135499  Numbers for which Sum_digits(odd positions) = Sum_digits(even
 *          positions).
 * https://oeis.org/A135499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 3719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135499: numbers where the sum of digits at odd positions
   equals the sum at even positions. The function generates the sequence
   by scanning integers in order and caching found terms. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"

/* static state: cache of found terms and the last candidate examined */
static seq_t *terms = NULL;
static size_t len   = 0;        /* number of terms stored */
static size_t cap   = 0;        /* allocated capacity */
static seq_t max_tested = 0;    /* last candidate examined */

/* check whether a candidate satisfies the digit‑sum equality condition */
static bool good_number(seq_t n)
{
    if (n == 0)
        return false;           /* zero never satisfies the condition */

    seq_t odd_sum  = 0;
    seq_t even_sum = 0;
    bool odd = true;           /* least significant digit is position 1 (odd) */

    seq_t tmp = n;
    while (tmp > 0)
    {
        seq_t d = tmp % 10;
        if (odd)
            odd_sum += d;
        else
            even_sum += d;
        odd = !odd;
        tmp /= 10;
    }
    return odd_sum == even_sum;
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour below offset is irrelevant */
        return 0;

    while (len < n)
    {
        seq_t candidate = max_tested + 1;
        if (good_number(candidate))
        {
            if (len + 1 > cap)
                grow_seq(&terms, &cap, len + 1);
            terms[len++] = candidate;
        }
        max_tested = candidate;
    }

    return terms[n - 1];
}
