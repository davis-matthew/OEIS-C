/*
 * A193995  a(n) = number of (distinct) terms required to determine whether n
 *          is a happy number or not (A007770 or A031177).
 * https://oeis.org/A193995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct terms in the happy-number iteration *
 *            starting from n (A007770 or A031177). The sequence includes the
 *            * initial value and counts all distinct iterates until a
 *            repetition is * first reached. The function returns the length
 *            of that distinct set
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9459 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* We only need to remember values up to 2000 because
     * digit_square_sum of any 64-bit number is <= 1620.
     */
    bool visited[2001] = {false};
    uint64_t cur = (uint64_t)n;
    seq_t count = 1;   /* the initial term is always counted */

    if (cur <= 2000) {
        visited[(unsigned)cur] = true;
    }

    while (1) {
        uint64_t next = digit_square_sum(cur);
        if (next <= 2000) {
            if (visited[(unsigned)next]) {
                break;                     /* repetition found */
            }
            visited[(unsigned)next] = true;
        }
        /* next cannot exceed 2000 after the first step, so no need to
         * handle the case next > 2000 here.
         */
        count++;
        cur = next;
    }

    return count;
}
