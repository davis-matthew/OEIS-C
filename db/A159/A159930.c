/*
 * A159930  Triangle read by rows: a(1,1)=1. a(m,n) = a(m-1,n) + (sum of all
 *          terms in row m-1), for n<m. a(m,m) = sum of all terms in row m-1.
 * https://oeis.org/A159930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(m,n) = a(m-1,n) + (m-1)!/2  (n<m),
   a(m,m) = m!/2,  a(1,1)=1.
   The sequence is read row-wise: a(1)=1, a(2)=2, a(3)=1, a(4)=5, a(5)=4, a(6)=3, ... */

/* Define the function a(idx_t n) which returns the n-th term of A159930.
   idx_t is an unsigned integer type (e.g., uint64_t),
   seq_t is an unsigned integer type of sufficient width. */
#ifndef __ASSEMBLER__
#include <stdbool.h>
#endif

seq_t a(idx_t n) {
    /* Handle the first term explicitly. */
    if (n == 1) return (seq_t)1;

    /* Find the row m such that m(m+1)/2 >= n. */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n) {
        ++m;
    }

    /* Compute the column index (1‑based). */
    idx_t prev = ((m - 1) * m) / 2;   /* sum of terms before row m */
    idx_t k = n - prev;               /* column within the row */

    /* Compute factorials and accumulate the needed sums. */
    seq_t fac = 1;                     /* t! */
    seq_t sum_after_k = 0;             /* sum of (t!/2) for t > k */
    seq_t a_k_value = 0;               /* a(k,k) when k < m */

    for (idx_t t = 1; t <= m; ++t) {
        fac = fac * (seq_t)t;          /* fac = t! */

        if (t == 1) {
            /* t==1 has a(k,k)=1 only when k==1 */
            if (t == k) {
                a_k_value = (seq_t)1;
            }
            continue;
        }

        /* t >= 2: t! is divisible by 2 */
        seq_t half = fac / (seq_t)2;

        if (t == k) {
            /* a(k,k) = (k!)/2 for k >= 2, for k==1 handled above */
            a_k_value = half;
        } else if (t > k) {
            sum_after_k += half;
        }
        /* For t < k nothing is accumulated */
    }

    /* Return the appropriate value. */
    if (k == m) {
        /* Last element of the row */
        return fac / (seq_t)2; /* fac == m! */
    } else {
        /* Any other element in the row */
        return a_k_value + sum_after_k;
    }
}
