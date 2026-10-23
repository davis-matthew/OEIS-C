/*
 * A132943  Concatenation of first n imperfect numbers.
 * https://oeis.org/A132943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of the first n imperfect numbers (positive
 *            integers that are not perfect)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

static int digit_len(idx_t x)
{
    /* x is uint64_t or larger, but for n ≤ 10000 values of x are ≤ 10000+6
       So the number of digits will be at most 5. */
    if (x < 10ULL)  return 1;
    if (x < 100ULL) return 2;
    if (x < 1000ULL) return 3;
    if (x < 10000ULL) return 4;
    if (x < 100000ULL) return 5;
    /* In the unlikely case x grows more, fall back to a loop */
    int d = 0;
    while (x != 0) {
        ++d;
        x /= 10ULL;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Known perfect numbers that can appear among the first ~10000 imperfect
       numbers.  They are all < 10 000 000, so they fit in uint64_t. */
    static const uint64_t perfects[] = {
        6ULL, 28ULL, 496ULL, 8128ULL,
        33550336ULL, 8589869056ULL
    };
    const size_t perfect_cnt = sizeof(perfects) / sizeof(perfects[0]);

    idx_t count = 0;         /* How many imperfect numbers we have seen */
    idx_t i = 1;             /* Current integer to examine */
    size_t pi = 0;           /* Index in the perfects array */

    seq_t result = 0;        /* Concatenated value built so far */

    while (count < n) {
        /* Skip perfect numbers */
        if (pi < perfect_cnt && i == perfects[pi]) {
            ++pi;
            ++i;
            continue;
        }

        /* Append the current imperfect number to the result */
        int d = digit_len(i);                     /* number of decimal digits */
        result = result * pow10((idx_t)d) + (seq_t)i;

        ++count;
        ++i;
    }

    return result;
}
