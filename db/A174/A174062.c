/*
 * A174062  Least possible sum of exactly n positive integers less than 2n
 *          such that none of the n integers divides another.
 * https://oeis.org/A174062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Start with the set { n , n+1 , … , 2n‑1 }.
   Repeatedly replace a chosen number by a smaller divisor x
   (with 2 ≤ x < n) if x divides exactly one chosen number.
   This greedy process terminates and yields the minimal‑sum
   antichain of size n consisting of numbers strictly less than 2n. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, not used for the sequence */

    /* Allocate workspace: indices 0 … 2n‑1 are needed. */
    size_t max = (size_t)(2 * n);       /* 2n ≤ 2^63 for all practical n */
    bool *chosen = (bool *)calloc(max, sizeof(bool));
    if (!chosen) return 0;              /* allocation failure unlikely */

    /* Initially choose all numbers from n to 2n‑1. */
    for (size_t i = (size_t)n; i < max; ++i)
        chosen[i] = true;

    /* Greedy replacement loop. */
    bool changed;
    do
    {
        changed = false;
        for (size_t x = 2; x < (size_t)n; ++x)      /* possible divisors */
        {
            if (chosen[x])
                continue;          /* x already in the set */

            size_t count = 0;
            size_t last  = 0;

            for (size_t m = x; m < max; m += x)
            {
                if (chosen[m])
                {
                    ++count;
                    last = m;
                    if (count > 1)  /* more than one multiple → skip */
                        break;
                }
            }

            if (count == 1)          /* unique multiple → replace */
            {
                chosen[last] = false;
                chosen[x]    = true;
                changed = true;
            }
        }
    } while (changed);

    /* Compute the sum of the chosen numbers. */
    seq_t sum = 0;
    for (size_t i = 1; i < max; ++i)
        if (chosen[i])
            sum += (seq_t)i;

    free(chosen);
    return sum;
}
