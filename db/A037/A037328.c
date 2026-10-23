/*
 * A037328  Numbers whose base-6 and base-8 expansions have the same digit
 *          sum.
 * https://oeis.org/A037328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑6 and base‑8 expansions have the same digit sum
 * (computed by brute‑force search). */
/* The sequence is defined as the increasing list of integers x such that
 * sum of digits of x in base 6 equals sum of digits of x in base 8. */

#include <stdbool.h>

/* Compute the sum of the digits of x in the given base (base = 6 or 8).
 * Both x and the sum are of type seq_t, which supports the C integer
 * operators and comparison with zero. */
static seq_t sum_digits_base(seq_t x, unsigned base)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % base;
        x /= base;
    }
    return sum;
}

/* Return the n-th element of the sequence.  The function performs a
 * simple search starting from 1, counting numbers that satisfy the
 * digit‑sum equality until the required index is reached. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* only defined for n >= 1 */

    seq_t candidate = 1;
    idx_t count = 0;
    seq_t result = 0;

    while (count < n) {
        seq_t s6 = sum_digits_base(candidate, 6);
        seq_t s8 = sum_digits_base(candidate, 8);

        if (s6 == s8) {
            result = candidate;
            ++count;
        }
        ++candidate;
    }

    return result;
}
