/*
 * A165054  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-6 Kaprekar map in A165051.
 * https://oeis.org/A165054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the total number of distinct values obtained by
 *            iterating * the base‑6 Kaprekar map starting from the integer n.
 *            The result * equals the length of the preperiodic part plus the
 *            length of the cycle * of the orbit of n
 * Range:     u8 n <= 227, u16 n <= 1296, u32 n <= 1296, u64 n <= 1296,
 *            u128 n <= 1296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t kaprekar(seq_t x)
{
    /* Decompose x into base‑6 digits */
    uint8_t digits[20];
    size_t len = 0;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t tmp = x;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 6);
            tmp /= 6;
        }
    }

    /* Count digits 0…5 */
    uint8_t freq[6] = {0};
    for (size_t i = 0; i < len; ++i) {
        ++freq[digits[i]];
    }

    /* Build the smallest and largest permutations */
    seq_t min = 0, max = 0;

    for (size_t d = 0; d < 6; ++d) {
        for (uint8_t k = 0; k < freq[d]; ++k) {
            min = min * 6 + d;
        }
    }

    for (int d = 5; d >= 0; --d) {
        for (uint8_t k = 0; k < freq[d]; ++k) {
            max = max * 6 + (uint8_t)d;
        }
    }

    return max - min;
}

seq_t a(idx_t n)
{
    /* Work in the type of the sequence values */
    seq_t x = (seq_t)n;

    /* Store seen values in a small array – the orbit is short. */
    seq_t seen[200];
    size_t count = 0;

    while (true) {
        /* check whether x is already seen */
        size_t i;
        for (i = 0; i < count; ++i) {
            if (seen[i] == x)
                break;
        }
        if (i < count)          /* repetition found */
            break;

        seen[count++] = x;
        x = kaprekar(x);
    }

    return (seq_t)count;
}
