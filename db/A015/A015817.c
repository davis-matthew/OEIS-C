/*
 * A015817  Numbers n such that phi(n + 1) | sigma(n) for n congruent to 1
 *          (mod 3).
 * https://oeis.org/A015817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m such that m ≡ 1 (mod 3) and φ(m + 1)
 *            divides σ(m)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   ensure_capacity, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence by
   incrementally testing candidates 1 mod 3.  The
   functions sigma_of and phi are provided by the
   utils library.  Results of the search are memoized
   so that the function works correctly for arbitrary
   calling order.  */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static data for memoization */
    static seq_t *values = NULL;   /* array of found terms      */
    static size_t count   = 0;     /* number of stored terms    */
    static size_t cap     = 0;     /* current allocation size   */
    static idx_t  cand    = 1;     /* next candidate 1 mod 3    */

    if (n == 0)            /* undefined for n=0, return 0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (count < (size_t)n) {
        /* Compute σ(cand) and φ(cand + 1).  The
           functions return seq_t values, and the
           modulo operation is defined for seq_t. */
        seq_t sigma_val = sigma_of(cand);
        seq_t phi_val   = phi(cand + (idx_t)1);

        if (phi_val != 0 && (sigma_val % phi_val) == 0) {
            /* Store this term */
            ensure_capacity(count + 1, &cap, (void **)&values, sizeof(seq_t));
            values[count++] = (seq_t)cand;
        }

        /* Next candidate that is ≡1 (mod 3) */
        cand += 3;
    }

    /* Return the requested term (1‑based index). */
    return values[n - 1];
}
