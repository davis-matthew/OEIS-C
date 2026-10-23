/*
 * A143545  a(n) = n unless n is a prime, in which case a(n) = 2n.
 * https://oeis.org/A143545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n  for non‑prime n,
 *               a(n) = 2*n for prime n (with a(1)=1).
 * The function works for any build configuration of seq_t and idx_t.
 */
#include <stdbool.h>

/* Helper: test primality of an idx_t value using trial division. */
static bool is_prime_custom(idx_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0)   return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    if (is_prime_custom(n)) {
        result = result * (seq_t)2;
    }
    return result;
}
