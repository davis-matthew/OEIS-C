/*
 * A061008  a(n) = Sum_{j=1..n} (-(n-1)! mod n).
 * https://oeis.org/A061008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1..n} (-(j-1)! mod j); for n > 3 this equals
 *            pi(n)+2. Initial values: a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 1608, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


static bool is_prime_small(idx_t n);

seq_t a(idx_t n)
{
    if (n <= 3u)                /* 0, 1, 2 */
        return (seq_t)(n - 1);

    /* Compute the prime counting function pi(n). */
    idx_t pi_count = 0;
    for (idx_t k = 2; k <= n; ++k)
        if (is_prime_small(k))
            ++pi_count;

    return (seq_t)pi_count + (seq_t)2;
}

static bool is_prime_small(idx_t v)
{
    if (v < 2)
        return false;
    if (v == 2 || v == 3)
        return true;
    if ((v & 1u) == 0 || v % 3 == 0)
        return false;

    idx_t i = 5;
    while (i <= v / i) {
        if (v % i == 0 || v % (i + 2) == 0)
            return false;
        i += 6;
    }
    return true;
}
