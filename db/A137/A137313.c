/*
 * A137313  Numbers k such that a type-6 Gaussian normal basis over GF(2^k)
 *          exists.
 * https://oeis.org/A137313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2058, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137313: numbers k such that a type‑6 Gaussian normal basis
   over GF(2^k) exists.  For each k we test
     p = 6k + 1  prime
     ord  = multiplicative order of 2 modulo p
   Let m = (p-1) / ord.  Then k is a term iff m divides 6
   and gcd(k, m) == 1.  The function returns the n‑th such k
   (with indexing starting at 1). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return the multiplicative order of 2 modulo prime p (p > 2). */
static uint64_t order_of_2(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t order = phi;
    uint64_t n = phi;
    uint64_t d = 2;

    /* factor n, reducing the order each time a prime divisor works */
    while (d * d <= n) {
        if (n % d == 0) {
            while (n % d == 0) n /= d;
            while (order % d == 0 &&
                   mod_pow((seq_t)2, order / d, (seq_t)p) == (seq_t)1)
                order /= d;
        }
        d += (d == 2) ? 1 : 2;    /* after 2, test only odd d */
    }
    if (n > 1) {
        while (order % n == 0 &&
               mod_pow((seq_t)2, order / n, (seq_t)p) == (seq_t)1)
            order /= n;
    }
    return order;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t k = 1; ; ++k) {
        uint64_t p = 6 * k + 1;
        if (!is_prime((idx_t)p))
            continue;
        uint64_t ord = order_of_2(p);
        uint64_t m = (p - 1) / ord;
        if (m > 6)
            continue;
        if (gcd((seq_t)k, (seq_t)m) != 1)
            continue;
        ++found;
        if (found == n)
            return (seq_t)k;
    }
}
