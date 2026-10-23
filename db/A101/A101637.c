/*
 * A101637  a(n) = 1 if n is a 4-almost prime, that is a product of exactly
 *          four (not necessarily distinct) primes, 0 otherwise.
 * https://oeis.org/A101637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is a product of exactly four (not necessarily
 *            distinct) primes, else 0
 * Range:     u8 n <= 999, u16 n <= 9831, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t temp = (seq_t)n;
    unsigned count = 0;

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % (seq_t)p == 0) {
            while (temp % (seq_t)p == 0) {
                ++count;
                temp /= (seq_t)p;
                if (count > 4) return 0;  /* early exit */
            }
        }
    }
    if (temp > 1) ++count;  /* remaining prime factor */
    return count == 4 ? (seq_t)1 : (seq_t)0;
}
