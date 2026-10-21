/*
 * A204131  Symmetric matrix based on f(i,j)=(2i-1 if i=j and 1 otherwise), by
 *          antidiagonals.
 * https://oeis.org/A204131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 for all n that are not on the main diagonal of the
 *            matrix M with M(i,i)=2*i-1 (otherwise a(n)=2*i-1). The position
 *            (i,j) of the nth entry is found by locating the anti‑diagonal it
 *            belongs to. The function enumerates the diagonals in increasing
 *            order of i+j: (1,1),(1,2),(2,1),(1,3),(2,2),...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* safety – the types are supplied by the build */

/*------------------------------------------------------------------*/
/*  a(n) – OEIS sequence A204131                                   */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal containing the nth element.
       The s‑th anti‑diagonal (s ≥ 2) contains (s‑1) elements.
       cum represents the total number of elements in all
       anti‑diagonals up to the previous one. */
    idx_t cum = 0;          /* elements before current diagonal */
    idx_t s   = 2;          /* current diagonal sum i+j  (≥2) */

    /* Incrementally find the first s with cum + (s-1) ≥ n */
    while (cum + (s - 1) < n) {
        cum += (s - 1);    /* add the length of this diagonal */
        ++s;               /* move to next diagonal */
    }

    /* Offset (1‑based) of the element inside this diagonal
       – i runs from 1 to s-1, j = s - i.               */
    idx_t offset = n - cum;   /* 1 ≤ offset ≤ s-1 */

    /* If offset equals s - offset (i==j) we are on the diagonal. */
    if (offset * 2 == s) {
        return (seq_t)(2 * offset - 1);  /* M(i,i) = 2i-1 */
    } else {
        return (seq_t)1;                 /* off‑diagonal entries are 1 */
    }
}
