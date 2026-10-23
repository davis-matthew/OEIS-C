/*
 * A030063  Fermat's Diophantine m-tuple: 1 + the product of any two distinct
 *          terms is a square.
 * https://oeis.org/A030063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030063: Fermat's Diophantine m‑tuple (finite integer sequence).  
   Known terms are 0, 1, 3, 8, 120.  For n > 4 the sequence is undefined;  
   this function returns 0 for such n. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t values[5] = {0, 1, 3, 8, 120};
    if (n < 5)
        return values[(size_t)n];
    return 0;
}
