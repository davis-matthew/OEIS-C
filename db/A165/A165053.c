/*
 * A165053  Length of preperiodic part of trajectory of n under iteration of
 *          the base-6 Kaprekar map in A165051.
 * https://oeis.org/A165053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 264, u16 n <= 1296, u32 n <= 1296, u64 n <= 1296,
 *            u128 n <= 1296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = length of the preperiodic part of the base‑6 Kaprekar
 * trajectory starting from n. Implements Floyd's cycle‑finding algorithm. */
#include <stdint.h>
#include <stdbool.h>

static seq_t kaprekar_next(seq_t x)
{
    unsigned int freq[6] = {0};

    if (x == 0) {
        freq[0] = 1;                /* special case: number 0 has one digit 0 */
    } else {
        seq_t t = x;
        while (t > 0) {
            unsigned int d = (unsigned int)(t % 6);
            freq[d]++;
            t /= 6;
        }
    }

    seq_t max_val = 0;
    for (int d = 5; d >= 0; --d) {
        unsigned int cnt = freq[d];
        while (cnt--) {
            max_val = max_val * 6 + d;
        }
    }

    seq_t min_val = 0;
    for (int d = 0; d <= 5; ++d) {
        unsigned int cnt = freq[d];
        while (cnt--) {
            min_val = min_val * 6 + d;
        }
    }

    return max_val - min_val;
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Floyd's cycle detection */
    seq_t tortoise = kaprekar_next(start);
    seq_t hare = kaprekar_next(kaprekar_next(start));
    while (tortoise != hare) {
        tortoise = kaprekar_next(tortoise);
        hare = kaprekar_next(kaprekar_next(hare));
    }

    /* Find preperiod (mu) */
    seq_t tortoise2 = start;
    seq_t hare2 = tortoise;
    uint64_t steps = 0;
    while (tortoise2 != hare2) {
        tortoise2 = kaprekar_next(tortoise2);
        hare2 = kaprekar_next(hare2);
        steps++;
    }
    return (seq_t)steps;
}
