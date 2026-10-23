/*
 * A165015  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-4 Kaprekar map in A165012.
 * https://oeis.org/A165015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (length of preperiod) + (length of cycle) of the
 *            trajectory of n under the base‑4 Kaprekar map (A165012)
 * Range:     u8 n <= 325, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t kaprekar_step(seq_t x)
{
    uint8_t freq[4] = {0};

    /* Count base‑4 digits of x. */
    if (x == 0) {
        freq[0] = 1;
    } else {
        seq_t t = x;
        while (t > 0) {
            uint8_t d = (uint8_t)(t % 4);
            freq[d]++;
            t /= 4;
        }
    }

    /* Construct the maximum (descending) and minimum (ascending)
       numbers from the same multiset of digits. */
    seq_t high = 0;
    for (int d = 3; d >= 0; d--)
        for (uint8_t k = 0; k < freq[d]; k++)
            high = high * 4 + (seq_t)d;

    seq_t low = 0;
    for (int d = 0; d <= 3; d++)
        for (uint8_t k = 0; k < freq[d]; k++)
            low = low * 4 + (seq_t)d;

    return high - low;
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Floyd's cycle‑finding algorithm. */
    seq_t tortoise = kaprekar_step(start);
    seq_t hare = kaprekar_step(kaprekar_step(start));

    /* 1. Find first meeting point. */
    while (tortoise != hare) {
        tortoise = kaprekar_step(tortoise);
        hare = kaprekar_step(kaprekar_step(hare));
    }

    /* 2. Find preperiod length mu. */
    size_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = kaprekar_step(tortoise);
        hare = kaprekar_step(hare);
        mu++;
    }

    /* 3. Find cycle length lambda. */
    size_t lambda = 1;
    hare = kaprekar_step(tortoise);
    while (hare != tortoise) {
        hare = kaprekar_step(hare);
        lambda++;
    }

    return (seq_t)(mu + lambda);
}
