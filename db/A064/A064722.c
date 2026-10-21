/*
 * A064722  a(1) = 0; for n >= 2, a(n) = n - (largest prime <= n).
 * https://oeis.org/A064722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - (largest prime <= n), with a(1)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    idx_t i = 5;
    /* use i <= x / i to avoid overflow */
    while (i <= x / i) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t p = n;
    while (!is_prime(p)) {
        --p;
        /* for n >= 2, a prime will be found before reaching 2 */
    }
    return (seq_t)(n - p);
}
