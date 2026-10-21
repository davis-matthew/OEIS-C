/*
 * A083075  Square array read by antidiagonals: T(n,k) = (k*(2*k+3)^n +
 *          1)/(k+1).
 * https://oeis.org/A083075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(k,n) = (k*(2*k+3)^n + 1)/(k+1) read by antidiagonals
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 113, u64 n <= 291,
 *            u128 n <= 806, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s * (s + 1)) / 2 <= n) {
        ++s;
    }
    --s; /* now s satisfies the inequality */

    /* t is the offset inside diagonal s */
    idx_t t = n - (s * (s + 1)) / 2;

    /* k is the row index (starts from 0) */
    idx_t k = s - t;

    /* Compute base = 2*k + 3 */
    seq_t base = (seq_t)(2 * k + 3);

    /* Compute pow_val = base^t */
    seq_t pow_val = big_pow(base, (seq_t)t);

    /* Use the simplified formula to avoid overflow:
       T = pow_val - (pow_val - 1) / (k + 1)   */
    seq_t denom = (seq_t)k + 1;
    seq_t result = pow_val - (pow_val - 1) / denom;

    return result;
}
