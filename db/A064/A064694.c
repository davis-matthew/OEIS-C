/*
 * A064694  Add column entries of the table with rows (1,2,0,0...),
 *          (0,3,4,5,0,0...), (0,0,6,7,8,9,0,0...),
 *          (0,0,0,10,11,12,13,14,0,0...), ...
 * https://oeis.org/A064694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 860, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064694:  Column-sum sequence from the triangular table.
   Formula used (simplified cubic form):

   a(n) = { (7·n³ + 30·n² + 32·n) / 48     if n is even
          { (7·n³ + 27·n² + 17·n – 3) / 48   if n is odd     }

   This is equivalent to the explicit formulas from the OEIS entry
   (a(2n‑1) = n·(n+1)·(7n‑4)/6  and  a(2n) = n·(n+1)·(7n+8)/6). */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert the index to the output type once */
    seq_t nn = (seq_t)n;

    /* Compute a(n) depending on parity of n */
    if ((n & 1ULL) == 0) {          /* n even */
        seq_t n2 = nn * nn;         /* n² */
        seq_t n3 = n2 * nn;         /* n³ */
        seq_t num = 7 * n3 + 30 * n2 + 32 * nn;
        return num / (seq_t)48;
    } else {                        /* n odd */
        seq_t n2 = nn * nn;         /* n² */
        seq_t n3 = n2 * nn;         /* n³ */
        seq_t num = 7 * n3 + 27 * n2 + 17 * nn;
        return (num - (seq_t)3) / (seq_t)48;
    }
}
