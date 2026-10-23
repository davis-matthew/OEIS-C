/*
 * A154297  Primes of the form (1+2+3+...+m)/21 = A000217(m)/21, for some m.
 * https://oeis.org/A154297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) are the primes p such that 21*p = m*(m+1) with m ≥ 1,
 *            equivalently p = m*(m+1)/42. The sequence is finite; only m =
 *            14, 21, 41, 42 give primes 5, 11, 41, 43
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    idx_t count = 0;
    /* For m > 42 the expression cannot be prime; a safe,
       generous upper bound is used to guarantee termination. */
    for (idx_t m = 1; m <= 5000; ++m) {
        seq_t prod = (seq_t)m * (m + 1);   /* m*(m+1) */
        if (prod % 42 == 0) {
            seq_t val = prod / 42;      /* m*(m+1)/42 */
            if (is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
    }
    return 0; /* n is beyond the length of the known sequence */
}
