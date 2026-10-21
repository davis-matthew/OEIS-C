/*
 * A064002  List pairs (i,j) with 1 <= i <= j in colexicographic order: (1,1),
 *          (1,2), (2,2), (1,3), (2,3), (3,3), (1,4), ... Let a(1) = 1. Then
 *          for n>=2 if the (n-1)-st pair is (i,j) then a(n) = a(i) + a(j) +
 *          1.
 * https://oeis.org/A064002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_j
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064002:  a(1)=1.
   For n≥2 let (i,j) be the pair that occurs at position n‑1 in the
   colexicographic list (1,1),(1,2),(2,2),(1,3),(2,3),(3,3),… .
   Then a(n)=a(i)+a(j)+1.  The table of already‑computed values is
   memoised so calls in any order are O(1) after the first build. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/find_j.h"

seq_t a(idx_t n)
{
    /* Static memoisation array indexed from 1.  Max. 10000 is enough
       for the intended test cases; enlarge if needed. */
    static seq_t A[10001] = {0};
    static idx_t up_to = 0;

    if (n == 0) return (seq_t)0;           /* undefined, guard */

    if (n > up_to) {
        /* initialise first element if not yet done */
        if (up_to == 0) {
            A[1] = (seq_t)1;
            up_to = 1;
        }
        /* build values up to n */
        for (idx_t k = up_to + 1; k <= n; ++k) {
            idx_t p   = k - 1;                     /* index of previous pair */
            idx_t j   = find_j(p);                 /* second element of pair */
            idx_t tprev = ((j - 1) * j) / 2;       /* T(j-1) */
            idx_t i = p - tprev;                   /* first element of pair */
            A[k] = A[i] + A[j] + (seq_t)1;
        }
        up_to = n;
    }
    return A[n];
}
