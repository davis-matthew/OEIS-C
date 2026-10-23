/*
 * A185508  Third accumulation array, T, of the natural number array A000027,
 *          by antidiagonals.
 * https://oeis.org/A185508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where the natural numbers are accumulated three
 *            times (A000027 → A144112 → A185508). The array entry T(i,j)
 *            (1‑based row i, column j) is given by
 * Range:     u8 n <= 12, u16 n <= 83, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   T(i,j) = ( i*(i+1)*(i+2)*j*(j+1)*(j+2)
                 * (4*i*i + (5*j+23)*i + 4*j*j + 3*j + 41) ) / 2880 */
   /* The sequence a(n) lists these entries by anti‑diagonals, starting
   with a(1)=T(1,1).  That is, for n≥1:
     let s minimal with s*(s‑1)/2 ≥ n          (s is row+column)
     let offset = n – (s‑1)*(s‑2)/2 – 1
     i = 1 + offset
     j = s - 1 - offset
   then a(n)=T(i,j). */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1. Locate the (i,j) coordinates on the anti‑diagonal that
       contains the n-th term.  All indices are 1‑based. */
    idx_t s = 2;              /* current sum i+j  */
    idx_t cum = 0;            /* cumulative terms up to previous diagonal */

    while ((cum + (s - 1)) < n) {
        cum += (s - 1);
        s++;
    }

    idx_t offset = n - cum - 1;
    idx_t i = 1 + offset;        /* row index  */
    idx_t j = (s - 1) - offset;  /* column index  */

    /* 2. Compute the entry T(i,j) using the rational formula.
       The final division by 2880 is exact.  We use an unsigned
       128‑bit intermediate to avoid overflow for n up to about
       10 000 (the benchmark bound). */
    unsigned __int128 big =
        (unsigned __int128)j * (j + 1) * (j + 2) *
        (unsigned __int128)i * (i + 1) * (i + 2);

    unsigned __int128 poly =
          (unsigned __int128)4 * i * i
        + (unsigned __int128)(5 * j + 23) * i
        + (unsigned __int128)4 * j * j
        + (unsigned __int128)3 * j
        + 41;

    big *= poly;
    big /= 2880;

    return (seq_t)big;
}
