/*
 * A166569  Numbers n such that 12*n+7 is not prime.
 * https://oeis.org/A166569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searching for the integer n such that 12*n+7 is not prime.
   Return the nth such integer, where a(1)=4, a(2)=7, ...            */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t count = 0;       /* number of found non‑prime candidates   */
    idx_t cand   = 1;      /* candidate integer n                    */

    while (true) {
        seq_t val = ((seq_t)12 * (seq_t)cand) + (seq_t)7;   /* 12*cand+7  */

        if (!isPrimeGeneric(val)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
