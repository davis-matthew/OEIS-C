/*
 * A129312  A minimal 2 X 2 subdeterminant array.
 * https://oeis.org/A129312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  a(n): minimal 2×2 subdeterminant computed from the array 
 *  T(i,j) = (2*i-1)*j - i + 1  
 *  where (i,j) are the positive integer pair obtained by enumerating
 *  all pairs in antidiagonal order (i.e. all (i,j) with i+j = const,
 *  increasing i).  The index n refers to this enumeration starting at 1.
 *  This implements the OEIS entry A129312.
 */

#include <math.h>      /* for sqrt */

seq_t a(idx_t n)
{
    /* Map index n to coordinates (i,j) in antidiagonal order.  */
    /* Find the smallest diagonal d such that d*(d+1)/2 >= n.
       The number of entries up to diagonal d-1 is (d-1)*d/2,
       so the position inside diagonal d is:  m = n - (d-1)*d/2.  */
    double r = sqrt((double)8 * n + 1.0);          /* sqrt(8n+1)  */
    idx_t k = (idx_t)((r - 1.0) / 2.0);            /* floor of (sqrt(8n+1)-1)/2  */
    idx_t tri = k * (k + 1) / 2;                  /* triangular number T(k) */

    idx_t d = (n <= tri) ? k : k + 1;             /* diagonal index (d = s-1) */

    idx_t prevTri = (d - 1) * d / 2;              /* T(d-1) */
    idx_t m = n - prevTri;                        /* 1‑based offset in diagonal */

    idx_t i = m;                                  /* row index */
    idx_t j = (d + 1) - m;                        /* column index (s = d+1) */

    /* Evaluate T(i,j) in the arbitrary‑precision type seq_t.  */
    seq_t val = ((seq_t)(2 * i - 1)) * (seq_t)j
                - (seq_t)i + (seq_t)1;
    return val;
}
