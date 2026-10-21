/*
 * A204246  Array given by f(i,1)=1, f(1,j)=1, f(i,i)=(i-1)!, and f(i,j)=0
 *          otherwise, read by antidiagonals.
 * https://oeis.org/A204246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   factorial, terms_in_diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204246:  a(n) is obtained by reading the matrix
   f(i,j) with f(i,1)=f(1,j)=1, f(i,i)=(i-1)!, f(i,j)=0 otherwise,
   along the antidiagonals (i+j constant), starting at n=1 for (1,1). */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/terms_in_diagonal.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, safe fallback */

    /* locate the diagonal that contains the n‑th term.
       Total terms up to diagonal s is (s‑1)s/2. */
    idx_t s = 2;
    idx_t cumulative = terms_in_diagonal(s);   /* = 1 for s = 2 */

    while (cumulative < n) {
        ++s;
        cumulative += terms_in_diagonal(s);
    }

    /* cumulative is the number of terms up to and including diagonal s */
    idx_t cumulative_before = cumulative - terms_in_diagonal(s);
    idx_t offset = n - cumulative_before;      /* 1‑based position inside the diagonal */

    /* first and last positions of each diagonal are always 1 */
    if (offset == 1 || offset == s - 1)
        return (seq_t)1;

    /* the main diagonal position (i==j) appears only when s is even,
       and its index inside the diagonal is offset == s/2. */
    if ((s & 1) == 0 && offset == s / 2)
        return factorial(offset - 1);          /* (i-1)! with i = offset */

    /* all other positions are zero */
    return (seq_t)0;
}
