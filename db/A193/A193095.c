/*
 * A193095  Number of times n can be written as concatenation of exactly two
 *          nonzero squares in decimal representation.
 * https://oeis.org/A193095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ways to write decimal representation of n * as
 *            concatenation of two non‑zero perfect squares. * Implementation:
 *            brute-force search over all split positions
 * Range:     u8 n <= 5293, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/sqrt_floor.h"

/* Count ways to write n as a concatenation of two nonzero squares. */
seq_t a(idx_t n) {
    /* Convert n to an array of decimal digits (MSD first). */
    char digits[20];           /* 64-bit integers need at most 20 decimal digits. */
    int len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = '0' + (tmp % 10);
        tmp /= 10;
    } while (tmp != 0);

    /* Reverse to get most‑significant digit first. */
    for (int i = 0; i < len / 2; ++i) {
        char c = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = c;
    }

    seq_t count = 0;

    /* Try every split between digits. */
    for (int split = 1; split < len; ++split) {
        /* Suffix must not start with a leading zero. */
        if (digits[split] == '0')
            continue;

        /* Build first part. */
        seq_t part1 = 0;
        for (int i = 0; i < split; ++i)
            part1 = part1 * 10 + (seq_t)(digits[i] - '0');

        /* Build second part. */
        seq_t part2 = 0;
        for (int i = split; i < len; ++i)
            part2 = part2 * 10 + (seq_t)(digits[i] - '0');

        if (part1 == 0 || part2 == 0)
            continue;

        /* Check if both parts are perfect squares. */
        seq_t r1 = sqrt_floor(part1);
        if (r1 * r1 != part1)
            continue;

        seq_t r2 = sqrt_floor(part2);
        if (r2 * r2 != part2)
            continue;

        ++count;
    }

    return count;
}
