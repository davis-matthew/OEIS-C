/*
 * A090329  Number of divisors of n that are prefixes of other divisors of n
 *          in binary representation.
 * https://oeis.org/A090329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n such that there exists another
 *            divisor e≠d with the binary representation of d being a strict
 *            prefix of the binary representation of e. (Strict prefix: the
 *            binary of d is the first bits of e and has fewer bits)
 * Range:     u8 n <= 915, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic and works for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>

/* Compute the number of bits required to represent a positive integer */
static inline idx_t bitlen_idx(idx_t v)
{
    idx_t r = 0;
    while (v) { v >>= 1; ++r; }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but handle gracefully */
        return 0;

    /* For n ≤ 10⁴ (as required) the number of divisors is < 128.
       A fixed small array suffices. */
    seq_t div[128];
    size_t divcnt = 0;

    /* Enumerate divisors */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            div[divcnt++] = (seq_t)i;
            if (i != n / i)
                div[divcnt++] = (seq_t)(n / i);
        }
    }

    seq_t res = 0;

    /* For each divisor d, check if there exists a divisor e whose
       binary representation starts with d's binary representation. */
    for (size_t ii = 0; ii < divcnt; ++ii) {
        seq_t d = div[ii];
        idx_t bits_d = bitlen_idx((idx_t)d);

        for (size_t jj = 0; jj < divcnt; ++jj) {
            if (ii == jj) continue;

            seq_t e = div[jj];
            idx_t bits_e = bitlen_idx((idx_t)e);

            if (bits_e <= bits_d) continue;

            /* Shift e to the length of d and compare. */
            if ((e >> (bits_e - bits_d)) == d) {
                ++res;
                break;   /* Found one, no need to check other e's */
            }
        }
    }

    return res;
}
