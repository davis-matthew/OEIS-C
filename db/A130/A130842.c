/*
 * A130842  Positive even integers that start with the letter 't' .
 * https://oeis.org/A130842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = first positive even integer whose English word starts
 *            with 't'. The first word depends only on the most significant
 *            non‑zero 3‑digit group
 * Range:     u8 n <= 41, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   starts_t_group
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/starts_t_group.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (offset 1).  For n == 0 we return 0. */
    if (n == 0) return 0;

    idx_t count = 0;
    seq_t cand  = 2;               /* first positive even integer */

    for (;; cand += (seq_t)2)      /* iterate over even numbers only */
    {
        /* Find the highest power of 1000 that does not exceed cand.
           Using division prevents overflow on the multiplication. */
        seq_t pow = 1;
        while (cand / 1000 >= pow)   /* while (cand ≥ pow*1000) */
            pow *= (seq_t)1000;

        seq_t g = cand / pow;        /* the most significant 3‑digit block */

        if (starts_t_group(g)) {
            ++count;
            if (count == n) return cand;
        }
    }
}
