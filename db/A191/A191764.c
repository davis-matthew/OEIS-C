/*
 * A191764  Integers that do not have a partition into a sum of an odd square
 *          and two (not necessarily distinct) triangular numbers.
 * https://oeis.org/A191764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* A191764: integers m*(m+1) where 2m+1 has no prime factor ≡3 (mod 4). */
static inline bool has_prime_factor_3mod4(idx_t k)
{
    /* iterate over odd divisors */
    for (idx_t i = 3; i <= k / i; i += 2) {
        if (k % i == 0) {
            if (i % 4 == 3) return true;   /* bad prime factor found */
            while (k % i == 0) k /= i;     /* strip this factor */
        }
    }
    /* remaining k is a prime factor > sqrt(original k) */
    if (k > 1 && k % 4 == 3) return true;
    return false;
}

static inline bool valid_m(idx_t m)
{
    idx_t k = 2 * m + 1;          /* k is odd */
    return !has_prime_factor_3mod4(k);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (valid_m(m)) {
            ++count;
            if (count == n) {
                return (seq_t)m * ((seq_t)m + 1);   /* pronic number */
            }
        }
    }
}
