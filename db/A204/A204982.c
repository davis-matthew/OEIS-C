/*
 * A204982  Least k such that n divides k!! - j!! for some j satisfying 1 <= j
 *          < k.
 * https://oeis.org/A204982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204982: a(n) = least k such that n divides k!! – j!! for some 1 ≤ j < k */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – not required for the sequence */

    /* Mark residues that have already been seen modulo n. */
    unsigned char *seen = (unsigned char*)calloc((size_t)n, 1);
    if (!seen) return 0;           /* out of memory – unlikely in tests */

    /* Current residues for even and odd double‑factorials. */
    idx_t res_even = 1 % n;        /* 0!! = 1 */
    idx_t res_odd  = 1 % n;        /* 1!! = 1 */

    /* The only j allowed at the start is j = 1. */
    seen[res_odd] = 1;

    idx_t k = 2;
    for (;; ++k)
    {
        if ((k & 1) == 0)          /* k even  ⇒ uses the even residue */
        {
            seq_t prod = (seq_t)k * (seq_t)res_even;
            seq_t rem  = prod % (seq_t)n;
            idx_t r    = (idx_t)rem;
            if (seen[r])
            {
                free(seen);
                return (seq_t)k;
            }
            seen[r] = 1;
            res_even = r;
        }
        else                       /* k odd   ⇒ uses the odd residue */
        {
            seq_t prod = (seq_t)k * (seq_t)res_odd;
            seq_t rem  = prod % (seq_t)n;
            idx_t r    = (idx_t)rem;
            if (seen[r])
            {
                free(seen);
                return (seq_t)k;
            }
            seen[r] = 1;
            res_odd = r;
        }
    }
}
