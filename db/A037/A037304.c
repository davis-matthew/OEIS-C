/*
 * A037304  Numbers whose base-2 and base-6 expansions have the same digit
 *          sum.
 * https://oeis.org/A037304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose binary and base‑6 digit sums are
 *            equal. The function searches integers in increasing order and
 *            returns the n‑th one
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether an integer satisfies the digit‑sum property. */
static bool satisfies(seq_t x)
{
    /* Small constants in seq_t form. */
    const seq_t zero = (seq_t)0;
    const seq_t one  = (seq_t)1;
    const seq_t two  = (seq_t)2;
    const seq_t six  = (seq_t)6;

    unsigned long long sum_bin = 0;
    unsigned long long sum_six = 0;
    seq_t tmp = x;

    /* Binary digit sum. */
    while (tmp != zero) {
        if ((tmp & one) != zero) {
            ++sum_bin;
        }
        tmp = tmp / two;
    }

    /* Base‑6 digit sum. */
    tmp = x;
    while (tmp != zero) {
        /* tmp % 6 fits in an unsigned long long */
        sum_six += (unsigned long long)(tmp % six);
        tmp = tmp / six;
    }

    return sum_bin == sum_six;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* undefined for n < 1 */
    }

    seq_t value = (seq_t)1;          /* start from 1 */
    idx_t count = 0;                 /* how many terms found so far */

    for (;; ++value) {
        if (satisfies(value)) {
            ++count;
            if (count == n) {
                return value;
            }
        }
    }
}
