/*
 * A202101  Numbers k such that 90*k + 59 is prime.
 * https://oeis.org/A202101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202101: a(n) is the n‑th non‑negative integer k such that 90*k + 59 is prime.
   Computed by testing successive k in ascending order. */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    idx_t count = 0;
    idx_t k     = 0;

    while (count < n) {
        seq_t val = (seq_t)((seq_t)90 * (seq_t)k + 59);
        if (isPrimeGeneric(val)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
    /* this line is never reached */
    return 0;
}
