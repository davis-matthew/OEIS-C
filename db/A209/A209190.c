/*
 * A209190  Least prime factor of reversal of digits of n.
 * https://oeis.org/A209190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = lpf( rev(n) )
 * Range:     u8 n <= 103, u16 n <= 10006, u32 n <= 12000, u64 n <= 12000,
 *            u128 n <= 12000, big any n
 * Validated: standard, pass, 98 terms through n = 9775 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    /* Reverse the decimal representation of n. */
    idx_t r = reverse_digits_idx(n);

    /* For r == 1 return 1, since 1 has no prime factor. */
    if (r <= (idx_t)1) {
        return (seq_t)1;
    }

    /* Least prime factor of the reversed number. */
    idx_t p = lpf(r);

    return (seq_t)p;
}
