/*
 * A205123  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205122) for which such j exists, and s(k)=k*(2^(k-1)).
 * https://oeis.org/A205123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205123:  for a given n, return the minimum j < k such that
   n divides s(k)-s(j), where k is the smallest index for which such
   a j exists and s(m)=m*2^(m-1).  The algorithm finds the first
   repeated residue of k*2^(k-1) modulo n; the earlier occurrence
   gives the desired j.  The implementation works for all build
   configurations (uint8_t, …, unsigned __int128, or arbitrarily
   precise big integer types). */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static idx_t *first_occ = NULL;
    static idx_t cap = 0;

    /* Allocate or resize the auxiliary array for this n. */
    if (n > cap) {
        idx_t *tmp = (idx_t *)realloc(first_occ, n * sizeof(idx_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        first_occ = tmp;
        cap = n;
    }

    /* Reset the array entries to 0 (meaning “not seen”). */
    memset(first_occ, 0, n * sizeof(idx_t));

    /* Current value of 2^(k-1) modulo n.  Start with k = 1. */
    idx_t pow2 = 1 % n;

    for (idx_t k = 1; k <= n + 1; ++k) {
        /* Residue r(k) = k * 2^(k-1) mod n. */
        idx_t rem = (k * pow2) % n;

        if (first_occ[rem])           /* a repeat found: earlier index stored */
            return (seq_t)first_occ[rem];

        first_occ[rem] = k;           /* record first occurrence */

        pow2 = (pow2 * 2) % n;         /* advance to next power of 2 */
    }

    /* The loop should always find a repeat because there are only n
       possible residues.  Returning 0 is a safe fallback. */
    return (seq_t)0;
}
