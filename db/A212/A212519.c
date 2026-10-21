/*
 * A212519  Number of (w,x,y,z) with all terms in {1,...,n} and w>2x and
 *          y>=3z.
 * https://oeis.org/A212519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 567, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212519: Number of (w,x,y,z) with all terms in {1,…,n},
   w > 2x and y >= 3z
   Formula: a(n) = ((floor((n-1)/2) * (n - floor((n-1)/2) - 1))
                    *
                    (floor(n/3) * (2*n - 3*floor(n/3) - 1) / 2) */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* compute r1 = floor((n-1)/2) safely */
    idx_t r1 = 0;
    if (n > 0) r1 = (n - 1) / 2;

    /* compute r2 = floor(n/3) */
    idx_t r2 = n / 3;

    /* if either factor is zero, the result is zero */
    if (r1 == 0 || r2 == 0) return 0;

    /* convert to seq_t for arithmetic */
    seq_t nseq  = (seq_t)n;
    seq_t r1seq = (seq_t)r1;
    seq_t r2seq = (seq_t)r2;

    /* A = r1 * (n - r1 - 1) */
    seq_t A = r1seq * (nseq - r1seq - 1);

    /* B = r2 * (2*n - 3*r2 - 1) / 2 */
    seq_t numB = r2seq * ( (seq_t)2 * nseq - (seq_t)3 * r2seq - 1 );
    seq_t B = numB / 2;

    /* result = A * B */
    return A * B;
}
