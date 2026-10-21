/*
 * A097012  a(n) = sigma(n) mod 210.
 * https://oeis.org/A097012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) mod 210
 * Range:     u8 n <= 849, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided sigma_of helper to compute the divisor sum. */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* undefined for n=0, return 0 for safety */
    }
    const seq_t MOD = 210;
    seq_t s = sigma_of(n);
    return s % MOD;
}
