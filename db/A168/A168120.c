/*
 * A168120  Square array T(n,k) read by antidiagonals in which column k lists
 *          each number A000009 followed by k-1 zeros, for k>0.
 * https://oeis.org/A168120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168120 – sequence computed by reading a square array
    T(n,k) by antidiagonals.
    Column k (k>0) lists the distinct‑part partition numbers
    (sequence A000009) with k‑1 zeros between consecutive
    entries.  Thus
          T(n,k) = { A000009[n/k]  if k>0 and n%k==0
                    { 0            otherwise
    The linear index n is read by first enumerating all pairs
    (row = s-k, col = k) with sum s = row+col increasing
    (the s‑th antidiagonal contains s entries for k=1..s).
    This implementation computes the necessary A000009 values
    on demand and is valid for any build using seq_t and idx_t
    defined by the surrounding framework. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- find the antidiagonal (s) containing the n‑th term ---- */
    idx_t s   = 0;
    idx_t tri = 0;
    while (tri <= n) {
        ++s;
        tri = (s * (s + 1)) / 2;        /*  triangle number C(s)  */
    }
    /* tri = C(s)  > n   and  C(s-1) = tri - s */
    idx_t prev_tri = tri - s;           /*  C(s-1)  */
    idx_t offset   = n - prev_tri;      /*  position inside diagonal 0 .. s-1  */
    idx_t k        = offset + 1;        /*  column index   (1 .. s)  */
    idx_t row      = s - k;             /*  corresponding row index  */

    /* ---- lazy construction of A000009 up to the needed index ---- */
    static seq_t *A = NULL;
    static idx_t  A_cap = 0;            /*  capacity in elements  */
    idx_t needed = 0;
    if (row > 0) {
        /* the largest A000009 index we might need is row/k  <= row  */
        needed = row;                  /*  maximum value of row  */
    } else {
        needed = 0;
    }

    if (A_cap <= needed) {
        idx_t new_cap = needed + 1;
        if (A == NULL) {
            A = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (A == NULL) return 0;   /*  allocation failure   */
        } else {
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /*  allocation failure   */
            A = tmp;
        }
        /* compute distinct‑part partition numbers up to needed */
        for (idx_t i = 0; i <= needed; ++i) A[i] = 0;
        A[0] = 1;
        for (idx_t i = 1; i <= needed; ++i) {
            for (idx_t j = needed; j >= i; --j)
                A[j] += A[j - i];
        }
        A_cap = new_cap;
    }

    /* ---- fetch the value from the table ---- */
    if (row % k == 0) {
        return A[row / k];
    } else {
        return 0;
    }
}
