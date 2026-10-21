/*
 * A240453  Greatest prime divisors of the palindromes with an even number of
 *          digits.
 * https://oeis.org/A240453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = greatest prime divisor of the palindrome formed by
 *            concatenating n and rev(n)
 * Range:     u8 n <= 9, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor, pow10, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/rev.h"          /* rev(idx_t) */
#include "../utils/pow10.h"        /* pow10(idx_t) */
#include "../utils/largest_prime_divisor.h" /* largest_prime_divisor(uint64_t) */

seq_t a(idx_t n)
{
    /* Convert index to native 64‑bit for intermediate calculations. */
    uint64_t nn = (uint64_t)n;

    /* Reverse of the decimal representation of n. */
    uint64_t rev_n = rev(nn);

    /* Count decimal digits of n (needed for the power of ten). */
    uint64_t tmp = nn;
    int digits = 0;
    do {
        digits++;
        tmp /= 10;
    } while (tmp > 0);

    /* Compute 10^digits as a seq_t. */
    seq_t pow = pow10((idx_t)digits);

    /* Build the palindrome: n followed by rev(n). */
    seq_t palindrome = (seq_t)nn * pow + (seq_t)rev_n;

    /* The palindrome fits in 64 bits for the required input range. */
    uint64_t pal64 = (uint64_t)palindrome;

    /* Greatest prime divisor of the 64‑bit value. */
    uint64_t gpf = largest_prime_divisor(pal64);

    return (seq_t)gpf;
}
