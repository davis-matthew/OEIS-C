/*
 * A137657  Primes that are simultaneously of the forms 24i+7 and 7j+24.
 * https://oeis.org/A137657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137657: primes of the form 168 k + 31 (k = 0, 1, 2, …) – computed by linear search. */

#include <stdlib.h>                       /* abort */
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) abort();  /* the sequence is 1‑indexed */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t p = ((seq_t)168 * (seq_t)k) + (seq_t)31;
        if (isPrimeGeneric(p)) {
            ++count;
            if (count == n) return p;
        }
    }
}
