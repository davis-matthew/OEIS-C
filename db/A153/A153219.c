/*
 * A153219  Numbers n such that 6*n + 7 is not prime.
 * https://oeis.org/A153219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isComposite6n7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by sequential search for the n‑th integer x with 6*x+7 composite */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isComposite6n7.h"

/* Exported sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset < 1, harmless fallback */
        return 0;

    idx_t count = 0;
    idx_t x = 0;

    while (count < n) {
        ++x;                    /* candidate integer */
        if (isComposite6n7(x))
            ++count;
    }
    return (seq_t)x;            /* cast to the sequence type */
}
