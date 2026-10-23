/*
 * A032313  "EGJ" (unordered, element, labeled) transform of 2,1,1,1...
 * https://oeis.org/A032313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   EGJ (unordered, element, labeled) transform of 2,1,1,1... EGF:
 *            (1 + x)*Π_{k>0} (1 + x^k/k!)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers for dynamic memory allocation of seq_t arrays (provided by the
   test harness). */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* Handle the offset case directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate temporary array c[0..n] for the coefficients of the
       product Π_{k>0} (1 + x^k/k!) scaled by n! (i.e. c_n = n! * coeff_n). */
    seq_t *c = alloc_seq_array((size_t)n + 1);
    if (!c) {
        return (seq_t)0;               /* allocation failure – unlikely in tests */
    }
    c[0] = (seq_t)1;                   /* base case */

    /* Incrementally build the product.  For each k we add the factor
       (1 + x^k/k!).  Using the combinatorial recurrence
       c_n = c_n + C(n,k) * c_{n-k}. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Initial binomial coefficient C(n,k).  Computed multiplicatively
           to keep all operations within the arithmetic of seq_t. */
        seq_t binom = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i) {
            binom = binom * (seq_t)(n - k + i) / (seq_t)i;
        }

        /* Update coefficients for all m from n down to k. */
        for (idx_t m = n; ; --m) {
            c[m] += binom * c[m - k];
            if (m == k) {
                break;                /* finished this k */
            }
            /* Prepare binom for the next smaller value of m:
               C(m-1,k) = C(m,k) * (m-k)/m. */
            binom = binom * (seq_t)(m - k) / (seq_t)m;
        }
    }

    /* Compute the final sequence value:
       a(n) = c[n] + n * c[n-1]  (recall a(n) = n! * (b_n + b_{n-1})
       with c[n] = n! * b_n). */
    seq_t result = c[n] + (seq_t)n * c[n - 1];

    free_seq_array(c);
    return result;
}
