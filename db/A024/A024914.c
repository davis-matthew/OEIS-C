/*
 * A024914  Numbers k such that 10*k - 3 is prime.
 * https://oeis.org/A024914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that 10*k - 3 is prime.
   The sequence is defined by A024914 in OEIS.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n == 0 we return 0 (no effect). */
    if (n == 0)
        return 0;

    idx_t k = 1;        /* candidate k          */
    idx_t cnt = 0;      /* number of found ks   */

    while (1) {
        /* 10*k-3 expressed in seq_t to avoid overflow of intermediate. */
        seq_t val = (seq_t)10 * (seq_t)k - (seq_t)3;

        if (isPrimeTrial(val)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)k;   /* found the n‑th term */
        }
        ++k;
    }
}
