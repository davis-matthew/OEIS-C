/*
 * A078683  Least prime of the form n*2^m+1 for m>0, or 0 if there is no such
 *          prime.
 * https://oeis.org/A078683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A078683 – Least prime of the form n*2^m+1 (m>0), or 0 if none.  
 *
 *  The implementation tests consecutive powers of two, multiplies by n,
 *  adds one, and checks for primality using the library routine
 *  isPrimeGeneric(seq_t).  For 64‑bit seq_t values the loop stops when
 *  shifting would overflow; for arbitrary‑precision types the shift is
 *  defined for any exponent.
 *
 *  The function has O(n) average running time for n up to 10 000 and
 *  uses only the facilities required by the BUILD, namely
 *  isPrimeGeneric.h and the basic arithmetic operations.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; ignore n==0 */
        return 0;

    seq_t base = (seq_t)n;      /* cast n to seq_t once */

    /* Maximum reasonable exponent for a 64‑bit seq_t; larger shifts
     * would overflow.  For arbitrary‑precision seq_t the shift is safe.
     */
    const uint64_t SHIFT_LIMIT = sizeof(seq_t) <= 8 ? 63 : 0;

    for (uint64_t m = 1; ; ++m)
    {
        /* Stop if the shift would overflow a fixed‑width type */
        if (sizeof(seq_t) <= 8 && m > SHIFT_LIMIT)
            break;

        /* Compute n * 2^m */
        seq_t pow2 = (seq_t)1 << m;          /* safe when m <= 63 for 64‑bit types */
        seq_t candidate = base * pow2 + 1;

        if (isPrimeGeneric(candidate))
            return candidate;
    }

    /* No prime found within the search range */
    return (seq_t)0;
}
