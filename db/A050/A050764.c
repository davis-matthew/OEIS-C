/*
 * A050764  Numbers of form k^k (for values of k see A050763) containing no
 *          pair of consecutive equal digits (probably finite).
 * https://oeis.org/A050764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the k-th integer k^k such that k^k contains no pair of
 *            consecutive equal digits (computed by a simple sequential
 *            search)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   no_consecutive_equal_digits, pow_k_to_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/pow_k_to_k.h"

seq_t a(idx_t n)
{
    /* Sequence starts at offset 1 */
    if (n == (idx_t)0) return (seq_t)0;
    idx_t k       = (idx_t)1;
    idx_t counter = (idx_t)0;
    while (1)
    {
        seq_t val = pow_k_to_k(k);
        if (no_consecutive_equal_digits(val))
        {
            counter++;
            if (counter == n) return val;
        }
        k++;
    }
}
