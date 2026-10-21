/*
 * A205787  Least positive integer k such that n divides k^3-j^3 for some j in
 *          [1,k-1].
 * https://oeis.org/A205787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A205787: 
   a(n) = least positive integer k such that n divides k³ – j³
   for some j with 1 ≤ j < k.
   For each n, the algorithm iterates k from 2 up to n+1
   while keeping a table of residues of j³ (mod n).
   Since j = k-n works for k = n+1, the answer is always ≤ n+1. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, but guard */

    /* allocate table of residues modulo n */
    size_t sz = (size_t)n;
    uint8_t *seen = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!seen) return 0;                  /* allocation failure */

    /* residue of j = 1 */
    seq_t r = 1 % (seq_t)n;
    seen[(size_t)r] = 1;

    /* search for k */
    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t km = k % (seq_t)n;               /* k modulo n (0≤km<n) */
        seq_t r_k = (km * km % (seq_t)n) * km % (seq_t)n; /* km³ mod n */

        size_t idx = (size_t)r_k;
        if (seen[idx]) {                      /* found j < k with same residue */
            free(seen);
            return (seq_t)k;
        }
        seen[idx] = 1;                         /* j = k will be considered for future k */
    }

    free(seen);
    /* fallback: should not happen, but for safety */
    return (seq_t)(n + 1);
}
