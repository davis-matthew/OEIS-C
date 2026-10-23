/*
 * A201301  Primes of the form 9*k^3+7.
 * https://oeis.org/A201301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 9*k^3+7 (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; 0 is unused */
    seq_t k = 0;                   /* k starts at 0 → 9*0^3+7 = 7 */
    idx_t count = 0;               /* number of primes found */

    while (1) {
        /* compute p = 9*k^3 + 7 */
        seq_t k2 = k * k;          /* k^2 */
        seq_t k3 = k2 * k;         /* k^3 */
        seq_t p = (seq_t)9 * k3 + (seq_t)7;

        if (is_prime_seq(p)) {
            ++count;
            if (count == n) return p;
        }
        ++k;
    }
}
