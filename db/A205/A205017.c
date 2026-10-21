/*
 * A205017  Least k such that n divides the k-th difference between distinct
 *          oblong numbers.
 * https://oeis.org/A205017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS A205017.
   Definition: For each n ≥ 1, a(n) is the smallest index k such that
   n divides the k‑th difference between distinct oblong numbers
   (oblong numbers are m(m+1)). The differences are enumerated
   by increasing second index j (j ≥ 2) and, for each j, by
   increasing first index i (1 ≤ i < j).  
   Thus the sequence of differences is:
       (1,2),(1,3),(2,3),(1,4),(2,4),(3,4),(1,5),(2,5),(3,5),(4,5),...
   The k‑th difference in this order is used in the definition.
*/
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1 – not used but safe */
    seq_t nseq = n;                 /* cast to seq_t for modulo */
    idx_t k = 1;                    /* current position in enumeration */

    for (idx_t j = 2; ; ++j) {     /* start with the second oblong number */
        seq_t base = (seq_t)j * ((seq_t)j + 1);   /* O_j = j(j+1) */
        seq_t diff = base - 2;                     /* i = 1 case: j(j+1)-1(2) */

        for (idx_t i = 1; i < j; ++i) {
            if (diff % nseq == 0)
                return (seq_t)k;             /* first k for which n|diff */

            /* move to next i: diff decreases by 2*(i+1) each step */
            ++k;
            if (i == j - 1) break;           /* last i for this j */
            diff -= 2 * ((seq_t)(i + 1));
        }
    }
}
