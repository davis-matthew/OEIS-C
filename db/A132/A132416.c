/*
 * A132416  Positive numbers which are powers of their last digit.
 * https://oeis.org/A132416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 57, u64 n <= 116,
 *            u128 n <= 231, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132416: Positive numbers which are powers of their last digit.
   Implementation: merge eight infinite series d^k with digit restrictions:
   d = 5 or 6 -> any k ≥ 1,
   d = 4 or 9 -> k odd,
   d = 2,3,7,8 -> k ≡ 1 (mod 4).
   The first term is 1. Subsequent terms are generated in ascending order,
   duplicates discarded.  The routine works for any seq_t; it calls
   big_pow from ../utils/big_pow.h to compute powers accurately. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

typedef struct {
    seq_t value;     /* current value d^k */
    uint64_t d;      /* base digit 2…9 */
    uint64_t step;   /* exponent increment (1,2, or 4) */
    uint64_t exp;    /* current exponent */
    bool exhausted;  /* true if further multiplication would overflow seq_t */
} Node;

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* the unique first term */

    /* initialise the eight active bases 2…9 */
    Node nodes[8];
    for (int i = 0; i < 8; ++i) {
        uint64_t d = i + 2;
        uint64_t step;
        if (d == 5 || d == 6)           step = 1;
        else if (d == 4 || d == 9)      step = 2;
        else                            step = 4;

        nodes[i].d          = d;
        nodes[i].step       = step;
        nodes[i].exp        = 1;
        nodes[i].value      = big_pow((seq_t)d, (seq_t)1);
        nodes[i].exhausted  = false;
    }

    seq_t last = 1; /* last distinct value emitted */
    idx_t  count = 1; /* we already have the first term */

    while (count < n) {
        /* find the node with the smallest not‑exhausted value */
        int minIdx = -1;
        seq_t minVal = 0;
        for (int i = 0; i < 8; ++i) {
            if (nodes[i].exhausted) continue;
            if (minIdx == -1 || nodes[i].value < minVal) {
                minIdx = i;
                minVal = nodes[i].value;
            }
        }
        if (minIdx == -1) break;          /* should not happen */

        seq_t v = nodes[minIdx].value;

        if (v != last) {                  /* new distinct term */
            ++count;
            if (count == n) return v;
            last = v;
        }

        /* advance this base to the next admissible exponent */
        nodes[minIdx].exp += nodes[minIdx].step;
        seq_t newVal = big_pow((seq_t)nodes[minIdx].d,
                               (seq_t)nodes[minIdx].exp);
        /* check for overflow: for increasing exponent the value should not
           decrease. If it does, we mark this series as exhausted.   */
        if (newVal < nodes[minIdx].value)
            nodes[minIdx].exhausted = true;
        else
            nodes[minIdx].value = newVal;
    }

    /* control should never reach here for a sane build */
    return (seq_t)0;
}
