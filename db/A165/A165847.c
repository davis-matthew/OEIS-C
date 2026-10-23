/*
 * A165847  Totally multiplicative sequence with a(p) = 26.
 * https://oeis.org/A165847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 26^Ω(n), where Ω(n) is the total number of prime factors
 *            of n (with multiplicity)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ω(n) for n >= 1.  For n = 1, Ω(1) = 0. */
    idx_t temp = n;
    idx_t omega = 0;

    /* Simple trial division – fast enough for n <= 10 000 (and still correct for larger values). */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        while (temp % p == 0) {
            temp /= p;
            omega++;
        }
    }
    if (temp > 1) /* Remaining prime factor > sqrt(temp) */
        omega++;

    /* Compute 26^omega using binary exponentiation.  seq_t may be any unsigned integer type or an
       arbitrary‑precision type that supports multiplication, addition, and bitwise operations. */
    seq_t result = 1;
    seq_t base   = (seq_t)26;
    idx_t exp    = omega;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
