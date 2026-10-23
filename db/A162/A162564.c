/*
 * A162564  a(n) is the smallest positive integer neither coprime to n! nor a
 *          divisor of n!.
 * https://oeis.org/A162564
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite not dividing n!. For n > 5, a(n) = 2 *
 *            next_prime(n); small n are special‐cased
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the first few special cases */
    if (n <= 5) {
        if (n == 2 || n == 3) return (seq_t)4;
        /* n == 4 or 5 */
        return (seq_t)9;
    }

    /* For n > 5, find the smallest prime greater than n */
    idx_t p = n + 1;
    while (!is_prime(p)) {
        ++p;
    }

    /* Return twice that prime; it is composite and divides n! not */
    return (seq_t)2 * (seq_t)p;
}
