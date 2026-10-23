/*
 * A173256  Partial sums of A001481.
 * https://oeis.org/A173256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{i=1..n} of the first n numbers that can be written as
 *                i = x^2 + y^2 for non‑negative integers x,y.
 * Each term is accumulated in seq_t; for the unsigned integer builds this is
 * equivalent to a(n) = Sum_{k=1..n} A001481(k).
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a non‑negative integer m can be expressed as a sum of two squares.
 * Criterion: in the prime factorisation of m every prime ≡ 3 (mod 4) must occur
 * with an even exponent.  0 and 1 satisfy this trivially. */
static bool is_sum_of_two_squares(idx_t m)
{
    if (m <= 1)               /* 0 = 0^2+0^2, 1 = 1^2+0^2 */
        return true;

    idx_t n = m;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            unsigned exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1))
                return false;
        }
    }
    /* If a prime factor > sqrt(n) remains, its exponent is 1. */
    if (n > 1 && (n & 3) == 3)
        return false;
    return true;
}

/* Compute the n‑th partial sum of the sums‑of‑two‑squares sequence. */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n<1; return 0 safely */
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t count = 0;

    for (idx_t x = 0; ; ++x) {
        if (is_sum_of_two_squares(x)) {
            sum += (seq_t)x;
            ++count;
            if (count == n)
                break;
        }
    }

    return sum;
}
