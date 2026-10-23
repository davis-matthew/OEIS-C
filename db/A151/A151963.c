/*
 * A151963  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the Kaprekar map in A151949.
 * https://oeis.org/A151963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct values in the decimal Kaprekar
 *            trajectory of n (pre‑period length + cycle length)
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_VIS 1024

/* Apply the decimal Kaprekar map to a single value. */
static seq_t kaprekar(seq_t x)
{
    if (x < 10)
        return 0;

    unsigned char d[64];
    size_t nd = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        d[nd++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Simple insertion sort for ascending order. */
    for (size_t i = 1; i < nd; ++i) {
        unsigned char key = d[i];
        size_t j = i;
        while (j > 0 && d[j-1] > key) {
            d[j] = d[j-1];
            --j;
        }
        d[j] = key;
    }

    seq_t high = 0, low = 0;
    for (size_t i = nd; i > 0; --i)       /* descending order for high   */
        high = high * 10 + d[i-1];
    for (size_t i = 0; i < nd; ++i)        /* ascending order for low    */
        low = low * 10 + d[i];

    return high - low;
}

/* External function implementing sequence A151963. */
seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;
    seq_t history[MAX_VIS];
    size_t used = 0;

    while (1) {
        for (size_t i = 0; i < used; ++i) {
            if (history[i] == current)
                return (seq_t)used;
        }
        history[used++] = current;
        current = kaprekar(current);
    }
}
