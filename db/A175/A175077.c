/*
 * A175077  Final number reached by iterating r -> A049711(r) starting at r =
 *          n.
 * https://oeis.org/A175077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = final number reached by iterating r -> r mod p, * where p
 *            is the largest prime less than r, until reaching 1 or 2
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* find the largest prime strictly less than n (n >= 2). */
static idx_t largest_prime_less_than(idx_t n)
{
    if (n <= 2) return 0;          /* no prime below 2 */
    for (idx_t x = n - 1; ; --x) { /* x is unsigned, so stop before underflow */
        if (x < 2) break;
        if (is_prime(x)) return x;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t r = n;
    while (r > 2) {
        idx_t p = largest_prime_less_than(r);    /* r > 2 guarantees p >= 2 */
        r = r % p;
    }
    return (seq_t)r;    /* r is 1 or 2 at this point */
}
