/*
 * A031186  Periods of sum of 5th powers of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = period of the cycle reached by iterating the sum of
 * fifth powers of the decimal digits of n.  The sequence is defined
 * for n ≥ 0.  The period is determined with Floyd's cycle–finding
 * algorithm. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

/* Sum of 5th powers of the digits 0..9 as a const array. */
static const seq_t DIG_POW5[10] = {
    (seq_t)0,   (seq_t)1,   (seq_t)32,  (seq_t)243,
    (seq_t)1024, (seq_t)3125, (seq_t)7776, (seq_t)16807,
    (seq_t)32768, (seq_t)59049
};

/* Compute f(x) = sum of 5th powers of the decimal digits of x.  The
 * input x is of type seq_t.  The function uses the helper
 * get_digits from the utilities. */
static seq_t next_value(seq_t x)
{
    uint8_t buf[64];
    size_t len;
    get_digits(x, buf, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += DIG_POW5[buf[i]];

    return sum;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    seq_t tortoise = next_value(start);
    seq_t hare = next_value(next_value(start));

    /* --- Find meeting point inside the cycle --- */
    while (tortoise != hare) {
        tortoise = next_value(tortoise);
        hare = next_value(next_value(hare));
    }

    /* --- Find the start of the cycle (mu) --- */
    tortoise = start;
    while (tortoise != hare) {
        tortoise = next_value(tortoise);
        hare = next_value(hare);
    }

    /* --- Find the length of the cycle (lambda) --- */
    seq_t period = 1;
    hare = next_value(tortoise);
    while (tortoise != hare) {
        hare = next_value(hare);
        period++;
    }

    return period;
}
