/*
 * A204925  a(n) is the index j < k such that n divides s(k) - s(j), where k
 *          is the least index (A204924) for which such j exists, and
 *          s=(1,2,3,5,8,13,...), the Fibonacci numbers.
 * https://oeis.org/A204925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204925: a(n) is the index j < k where k is the smallest index such that
   s(k) ≡ s(j) (mod n) for the series s(1)=1, s(2)=2, s(k)=s(k-1)+s(k-2).
   The function returns that j.
*/
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n = 0, return 0 */

    /* Allocate array for first occurrence of each residue modulo n.
       We use 0 to mean “unvisited” and store indices starting at 1. */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first)
        return 0;                     /* allocation failed */

    memset(first, 0, (size_t)n * sizeof(idx_t));

    /* s(1) = 1 */
    idx_t r1 = 1 % n;
    first[r1] = 1;

    /* s(2) = 2 */
    idx_t r2 = 2 % n;
    if (first[r2] != 0) {             /* immediate repeat (n = 1 case) */
        seq_t res = (seq_t)first[r2];
        free(first);
        return res;
    }
    first[r2] = 2;

    idx_t prev2 = r1;                  /* s(k-2) for k = 3 */
    idx_t prev1 = r2;                  /* s(k-1) for k = 3 */

    for (idx_t k = 3; ; ++k) {
        idx_t r = (prev1 + prev2) % n; /* s(k) modulo n */

        if (first[r] != 0) {           /* s(k) repeats an earlier value */
            seq_t res = (seq_t)first[r];
            free(first);
            return res;
        }

        first[r] = k;                  /* first time this residue appears */

        /* advance the Fibonacci-like sequence */
        prev2 = prev1;
        prev1 = r;
    }
}
