/*
 * A057218  a(n) = least prime of the form n*k! + 1.
 * https://oeis.org/A057218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 30, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least prime of the form n*k! + 1 for k ≥ 1 */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t fact = 1;                 /* factorial = 1! initially (k = 1) */
    for (idx_t k = 1;; ++k) {
        fact *= (seq_t)k;           /* increment factorial to k!   */
        seq_t candidate = (seq_t)n * fact + 1;  /* n*k! + 1          */
        if (is_prime_big(candidate))
            return candidate;       /* found the least prime          */
    }
}
