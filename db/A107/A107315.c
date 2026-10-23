/*
 * A107315  Odd numbers written backwards and sorted.
 * https://oeis.org/A107315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose first decimal digit is
 *            odd and whose last decimal digit is non‑zero
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int oddDigits[5] = {1, 3, 5, 7, 9};

/* Compute 10^e in seq_t */
static seq_t pow10_seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* Compute 10^e in idx_t (used for counting, e is small) */
static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the length of the number that contains the n-th term */
    idx_t cumulative = 0;
    idx_t len = 1;
    while (1) {
        idx_t countLen;
        if (len == 1) {
            countLen = 5;                     /* 1,3,5,7,9 */
        } else {
            idx_t midPow = pow10_idx(len - 2); /* 10^(len-2) */
            countLen = 5 * 9 * midPow;        /* leading odd digit * last digit (1‑9) */
        }
        if (cumulative + countLen >= n)
            break;
        cumulative += countLen;
        ++len;
    }

    /* Offset inside the current length (0‑based) */
    idx_t k = n - cumulative - 1;

    if (len == 1) {
        /* Single‑digit numbers : 1,3,5,7,9 */
        return (seq_t)oddDigits[(int)k];
    }

    /* For len ≥ 2 */
    idx_t midPow   = pow10_idx(len - 2);                 /* 10^(len-2) */
    idx_t groupSize = 9 * midPow;                       /* numbers per leading digit */
    idx_t groupIdx  = k / groupSize;                    /* 0…4 */
    idx_t offset    = k % groupSize;                    /* inside that group */

    seq_t leading   = (seq_t)oddDigits[(int)groupIdx];

    /* last digit: 1‑9 */
    idx_t lastIdx   = offset % 9;                       /* 0…8 */
    seq_t lastDigit = (seq_t)(lastIdx + 1);

    /* middle digits: 0 … 10^(len-2)-1 */
    seq_t middleDigits = (seq_t)(offset / 9);

    /* Assemble the result */
    seq_t powLenMinus1 = pow10_seq(len - 1);            /* 10^(len-1) */
    seq_t result = leading * powLenMinus1
                   + middleDigits * 10
                   + lastDigit;

    return result;
}
