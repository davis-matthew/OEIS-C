/*
 * A110131  Determinant of n X n matrix M_{i,j} = 2^i*P_i(j), where P_i(j) is
 *          the Legendre polynomial of order i at j and i and j are 0-based.
 * https://oeis.org/A110131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=1}^{n-1} ∏_{j=i}^{n-1} (i+j) (OEIS A110131)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n <= 0 */

    /* Memoization for increasing queries.
       current_val stores a(current_n).  */
    static seq_t current_val = 1;
    static idx_t current_n   = 1;

    /* a(1) = 1 */
    if (n == 1)
        return 1;

    if (n >= current_n) {
        /* increase from current_n up to n using recurrence
           a(k+1) = a(k) * ∏_{t=k+1}^{2k} t                           */
        for (idx_t k = current_n; k < n; ++k) {
            for (idx_t t = k + 1; t <= 2 * k; ++t)
                current_val *= (seq_t)t;
        }
        current_n = n;
        return current_val;
    } else {
        /* n < current_n : recompute from scratch (rare case) */
        seq_t result = 1;
        for (idx_t i = 1; i <= n - 1; ++i) {
            for (idx_t j = i; j <= n - 1; ++j)
                result *= (seq_t)(i + j);
        }
        current_val = result;
        current_n   = n;
        return result;
    }
}
