/*
 * A164996  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-3 Kaprekar map in A164993.
 * https://oeis.org/A164996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 454, u16 n <= 2187, u32 n <= 2187, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Validated: standard, pass, 121 terms through n = 2048 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164996: Length of preperiodic part + length of cycle of the base‑3
   Kaprekar map.  This implementation uses Floyd's cycle‑finding
   algorithm so that it works for arbitrary‑size seq_t values and
   remains O(1) memory. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the next value of the base‑3 Kaprekar map. */
static seq_t
next_kaprekar(seq_t n)
{
    /* Extract base‑3 digits of n into a temporary array.  The number
       of digits is at most log_3(n)+1, which is tiny for any value
       that fits in a 64‑bit integer and still reasonable for
       arbitrary‑precision values. */
    size_t len = 0;
    size_t cap = 4;
    uint8_t *dig = (uint8_t *)malloc(cap);
    if (!dig)
        return (seq_t)0;               /* unlikely to happen */

    if (n == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        seq_t tmp = n;
        while (tmp > 0) {
            if (len == cap) {
                cap <<= 1;
                uint8_t *nd = (uint8_t *)realloc(dig, cap);
                if (!nd) { free(dig); return (seq_t)0; }
                dig = nd;
            }
            dig[len++] = (uint8_t)(tmp % (seq_t)3);
            tmp /= (seq_t)3;
        }
    }

    /* Sort digits (insertion sort – O(k²) where k = len, k is tiny). */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = dig[i];
        size_t j = i;
        while (j > 0 && dig[j-1] > key) {
            dig[j] = dig[j-1];
            --j;
        }
        dig[j] = key;
    }

    /* Build smallest and largest numbers from the sorted digits. */
    seq_t min_val = 0, max_val = 0;
    for (size_t i = 0; i < len; ++i)
        min_val = min_val * (seq_t)3 + (seq_t)dig[i];
    for (size_t i = len; i > 0; --i)
        max_val = max_val * (seq_t)3 + (seq_t)dig[i-1];

    free(dig);
    return max_val - min_val;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Floyd's cycle detection. */
    seq_t tortoise = next_kaprekar(start);
    seq_t hare = next_kaprekar(next_kaprekar(start));

    /* Find first meeting point. */
    while (tortoise != hare) {
        tortoise = next_kaprekar(tortoise);
        hare     = next_kaprekar(next_kaprekar(hare));
    }

    /* Find preperiod length mu. */
    size_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = next_kaprekar(tortoise);
        hare     = next_kaprekar(hare);
        ++mu;
    }

    /* Find cycle length lambda. */
    size_t lambda = 1;
    hare = next_kaprekar(tortoise);
    while (tortoise != hare) {
        hare = next_kaprekar(hare);
        ++lambda;
    }

    return (seq_t)mu + (seq_t)lambda;
}
