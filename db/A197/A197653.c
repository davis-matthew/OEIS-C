/*
 * A197653  Triangle by rows T(n,k), showing the number of meanders with
 *          length (n+1)*4 and containing (k+1)*4 Ls and (n-k)*4 Rs, where Ls
 *          and Rs denote arcs of equal length and a central angle of 90
 *          degrees which are positively or negatively oriented.
 * https://oeis.org/A197653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 58, u64 n <= 179, u128 n <= 644,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (conjectured):

   T(1,n,k) = binom(n,k)

   T(2,n,k) = T(1,n,k)^2 + T(1,n,k)*T(1,n,n-1-k),   0 ≤ k < n
   T(2,n,n) = 1

   T(3,n,k) = T(1,n,k)^3 + T(1,n,k)*T(2,n,n-1-k),   0 ≤ k < n
   T(3,n,n) = 1

   T(4,n,k) = T(1,n,k)^4 + T(1,n,k)*T(3,n,n-1-k),   0 ≤ k < n
   T(4,n,n) = 1

   The sequence a(n) is the row‑wise enumeration of the triangle
   T(4,n,k) with offset 0 (a(0)=1).  Thus a(n) is the (n‑th) element
   when the rows 0,1,2,… are listed left to right.  For a given
   index n the corresponding row r satisfies

          (r+1)(r+2)/2  >  n
          r(r+1)/2     ≤  n

   and the column k is  k = n – r(r+1)/2.  The value returned is
   T(4,r,k).  */

#include <stddef.h>
#include <stdlib.h>

/* The types seq_t and idx_t are provided by the build system.  */

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.  */
    idx_t r = 0;
    while ( ( (r + 1) * (r + 2) ) / 2 <= n )
        ++r;

    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;              /* 0 ≤ k ≤ r */

    /* Allocate temporary arrays for this row.  */
    size_t sz = (size_t)r + 1;
    seq_t *C  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *T2 = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *T3 = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *T4 = (seq_t *)malloc(sz * sizeof(seq_t));

    /* C[j] = binom(r , j) computed multiplicatively.  */
    C[0] = 1;
    for (size_t j = 1; j < sz; ++j)
        C[j] = C[j-1] * (seq_t)(r - j + 1) / (seq_t)j;

    /* T2[j] = C[j]^2 + C[j]*C[j+1] for j<r; T2[r] = 1.  */
    for (size_t j = 0; j + 1 < sz; ++j)
        T2[j] = C[j] * C[j] + C[j] * C[j+1];
    T2[sz-1] = 1;

    /* T3[j] = C[j]^3 + C[j]*T2[r-1-j] for j<r; T3[r] = 1.  */
    for (size_t j = 0; j + 1 < sz; ++j)
        T3[j] = C[j] * C[j] * C[j] + C[j] * T2[sz-2 - j];
    T3[sz-1] = 1;

    /* T4[j] = C[j]^4 + C[j]*T3[r-1-j] for j<r; T4[r] = 1.  */
    for (size_t j = 0; j + 1 < sz; ++j)
        T4[j] = C[j] * C[j] * C[j] * C[j] + C[j] * T3[sz-2 - j];
    T4[sz-1] = 1;

    seq_t result = T4[k];

    /* Clean up.  */
    free(C);
    free(T2);
    free(T3);
    free(T4);

    return result;
}
