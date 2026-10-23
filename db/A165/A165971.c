/*
 * A165971  The n-th odd nonprime minus the n-th even nonprime.
 * https://oeis.org/A165971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A014076(n) - A163300(n) * A014076(n) is the n‑th odd
 *            nonprime number (1, 9, 15, …), * A163300(n) is the n‑th even
 *            nonprime number (0, 4, 6, 8, …)
 * Range:     u8 n <= 237, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for unexpected calls */

    /* Find the n-th odd nonprime */
    idx_t count = 0;
    seq_t cand = (seq_t)1;          /* start with 1, the first odd nonprime */
    while (count < n) {
        if (cand == (seq_t)1 || !is_prime_big(cand)) {
            count++;
            if (count == n) break;   /* we have found the n‑th value */
        }
        cand += (seq_t)2;           /* proceed to the next odd number */
    }
    seq_t odd_val = cand;

    /* Compute the n‑th even nonprime */
    seq_t even_val = (n == 1) ? (seq_t)0 : (seq_t)2 * (seq_t)n;

    return odd_val - even_val;
}
