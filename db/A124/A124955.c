/*
 * A124955  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) where A is the 27×27 adjacency matrix with *
 *            A[i][j] = 1 if |i-j| <= 2 and 0 otherwise. The sequence
 *            satisfies a * linear recurrence of order 27 derived from the
 *            characteristic polynomial * of A. The recurrence is: * a_n =
 *            Σ_{j=1}^{27} (-1)^{j+1} e_j * a_{n-j}, * where e_j are t...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
 
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t a_vals[28];   /* a_vals[0..27] = a(0)..a(27)            */
    static seq_t e[28];        /* e[0..27] : elementary symmetric sums  */
 
    if (!init) {
        /* Build adjacency matrix (0/1) for the 27‑vertex graph. */
        seq_t A[27][27];
        for (idx_t i = 0; i < 27; ++i) {
            for (idx_t j = 0; j < 27; ++j) {
                idx_t d = (i > j) ? (i - j) : (j - i);
                A[i][j] = (d <= 2) ? 1 : 0;
            }
        }
 
        /* Compute trace(A^k) for k = 1..27. */
        seq_t M[27][27];
        for (idx_t i = 0; i < 27; ++i)
            for (idx_t j = 0; j < 27; ++j)
                M[i][j] = A[i][j];
 
        a_vals[0] = 1;    /* Empty word */
 
        for (idx_t step = 1; step <= 27; ++step) {
            seq_t tr = 0;
            for (idx_t i = 0; i < 27; ++i)
                tr += M[i][i];
            a_vals[step] = tr;
 
            if (step < 27) {      /* M = M*A  for next power */
                seq_t R[27][27];
                for (idx_t i = 0; i < 27; ++i) {
                    for (idx_t j = 0; j < 27; ++j) {
                        seq_t sum = 0;
                        for (idx_t k = 0; k < 27; ++k)
                            sum += M[i][k] * A[k][j];
                        R[i][j] = sum;
                    }
                }
                for (idx_t i = 0; i < 27; ++i)
                    for (idx_t j = 0; j < 27; ++j)
                        M[i][j] = R[i][j];
            }
        }
 
        /* Compute elementary symmetric sums e[1..27] via Newton's identities.
         * Here p_k = a_vals[k] = trace(A^k).  e[0] = 1.                */
        e[0] = 1;
        for (idx_t j = 1; j <= 27; ++j) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= j; ++i) {
                seq_t term = e[j - i] * a_vals[i];
                if (i & 1)            /* i odd => + */
                    sum += term;
                else                  /* i even => - */
                    sum -= term;
            }
            e[j] = sum / (seq_t)j;   /* exact division */
        }
 
        init = true;
    }
 
    if (n <= 27) {
        return a_vals[(idx_t)n];
    }
 
    /* For n > 27 use the linear recurrence.  buf holds the last 27
     * values: buf[0] = a_{n-27}, buf[1] = a_{n-26}, ..., buf[26] = a_{n-1}. */
    seq_t buf[27];
    for (idx_t i = 0; i < 27; ++i)
        buf[i] = a_vals[i];
 
    for (idx_t step = 27; step <= n; ++step) {
        seq_t sumPos = 0;   /* odd j terms (coefficient +1) */
        seq_t sumNeg = 0;   /* even j terms (coefficient -1) */
        for (idx_t j = 1; j <= 27; ++j) {
            idx_t idx = 27 - j;                 /* a_{step-j} */
            seq_t prod = e[j] * buf[idx];
            if (j & 1)
                sumPos += prod;
            else
                sumNeg += prod;
        }
        seq_t new_val = sumPos - sumNeg;      /* Recurrence: + odd, - even */
 
        /* Shift buffer left and insert new_val at the end. */
        for (idx_t i = 0; i < 26; ++i)
            buf[i] = buf[i + 1];
        buf[26] = new_val;
    }
 
    return buf[26];
}
