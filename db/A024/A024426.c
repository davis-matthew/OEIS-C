/*
 * A024426  a(n) = floor((1/n)*(S(n,1) + S(n,2) + ... + S(n,n))), where S(i,j)
 *          are Stirling numbers of second kind.
 * https://oeis.org/A024426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( B(n) / n ) where B(n) = sum_{k=1..n} S(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for n >= 1.  The return value is B(n)/n using integer
   division (floor).  For n == 0 the function returns 0 as a guard. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    /* ----- static cache of Bell numbers B[0..] ---- */
    static seq_t *B_vals = NULL;
    static size_t B_cap = 0;               /* capacity of B_vals */
    static size_t B_len = 0;               /* number of stored values */

    /* ----- static cache of previous Stirling row S_prev[0..] ---- */
    static seq_t *S_prev = NULL;
    static size_t S_prev_len = 0;          /* length of S_prev (row n-1 + 1) */

    /* ----- initialization (only once) ----- */
    if (B_len == 0) {
        B_cap = 1;
        B_vals = (seq_t*) malloc(sizeof(seq_t) * B_cap);
        B_vals[0] = (seq_t)1;              /* B(0) = 1 */

        B_len = 1;

        S_prev_len = 1;
        S_prev = (seq_t*) malloc(sizeof(seq_t) * S_prev_len);
        S_prev[0] = (seq_t)1;              /* S(0,0) = 1 */
    }

    /* ----- extend cache up to n ----- */
    for (idx_t m = (idx_t)B_len; m <= n; ++m) {
        /* Allocate array for S(m,0..m) */
        size_t new_len = (size_t)m + 1;
        seq_t *S_cur = (seq_t*) calloc(new_len, sizeof(seq_t));

        /* Compute Stirling numbers for k = 1 .. m-1 */
        for (size_t k = 1; k + 1 < new_len; ++k) {
            seq_t mul = S_prev[k] * (seq_t)k;    /* k * S(m-1,k) */
            S_cur[k] = mul + S_prev[k - 1];      /* + S(m-1,k-1) */
        }
        S_cur[m] = (seq_t)1;                   /* S(m,m) = 1 */

        /* Sum to get B(m) = Σ_{k=1..m} S(m,k) */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k < new_len; ++k) sum += S_cur[k];

        /* Store B(m) */
        if ((size_t)m >= B_cap) {
            B_cap = ((size_t)m + 1) * 2;
            B_vals = (seq_t*) realloc(B_vals, B_cap * sizeof(seq_t));
        }
        B_vals[m] = sum;
        B_len = (size_t)m + 1;

        /* Advance to next row */
        free(S_prev);
        S_prev = S_cur;
        S_prev_len = new_len;
    }

    /* ----- return floor(B(n)/n) -- integer division does the floor ----- */
    return B_vals[n] / (seq_t)n;
}
