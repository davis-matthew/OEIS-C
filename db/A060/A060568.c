/*
 * A060568  Number of primes between n and R(n) where R(n) (A004086) is the
 *          digit reversal of n.
 * https://oeis.org/A060568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of primes p such that min(n,R(n)) < p <
 *            max(n,R(n))
 * Range:     u8 n <= 1002, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9732 (2026-11-01)
 * Helpers:   is_prime, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where R(n) is the digit reversal of n (no leading zeros) */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    idx_t r = reverse_digits_idx(n);                 /* digit reversal of n */
    idx_t low  = (n < r) ? n : r;     /* smaller endpoint */
    idx_t high = (n < r) ? r : n;     /* larger endpoint */

    if (high <= low + 1)              /* no integers strictly between */
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t x = low + 1; x < high; ++x)
    {
        if (is_prime(x))
            ++count;
    }
    return count;
}
