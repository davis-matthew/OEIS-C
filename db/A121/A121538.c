/*
 * A121538  Increasing sequence: "if n appears then a*n+b doesn't", case
 *          a(1)=1, a=2, b=1.
 * https://oeis.org/A121538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121538: a(n) is the nth positive integer that satisfies
 * one of the following conditions:
 * 1) n is even;
 * 2) n+1 is a power of two with an odd exponent;
 * 3) n+1 = 2^k * (2*t+1) where k is even and t>0.
 * This implementation enumerates integers in order and selects the
 * ones that meet the conditions, stopping when the nth such integer is reached.
 */

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* True iff x is in the set defined by the conditions above. */
static bool in_set(seq_t x)
{
    /* Even numbers are always in the set. */
    if ((x & (seq_t)1) == (seq_t)0)
        return true;

    /* Odd x: analyze y = x + 1. */
    seq_t y   = x + (seq_t)1;
    seq_t tmp = y;
    unsigned v2 = 0;

    /* Count trailing zeros of y (i.e., the exponent of 2 in y). */
    while ((tmp & (seq_t)1) == (seq_t)0) {
        tmp /= (seq_t)2;
        ++v2;
    }

    /* If tmp == 1 then y is a pure power of two. */
    if (tmp == (seq_t)1) {
        /* Condition 2: power of two with odd exponent. */
        if (v2 & 1)
            return true;
    } else {
        /* Condition 3: y = 2^k * odd, k even, odd > 1. */
        if ((v2 & 1) == 0 && tmp > (seq_t)1)
            return true;
    }

    return false;
}

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    assert(n > (idx_t)0);

    idx_t count = 0;
    seq_t cand  = (seq_t)1;

    while (true) {
        if (in_set(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand += (seq_t)1;
    }
}
