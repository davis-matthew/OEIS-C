/*
 * A191004  Number of ways to write n = p+q+(n mod 2)q, where p is an odd
 *          prime and q<=n/2 is a prime such that JacobiSymbol[q,n]=1 if n is
 *          odd, and JacobiSymbol[(q+1)/2,n+1]=1 if n is even.
 * https://oeis.org/A191004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as n = p + q + (n mod 2) *
 *            q, where p is an odd prime, q ≤ n/2 is a prime,
 *            JacobiSymbol[q,n] = 1 if n is odd, JacobiSymbol[(q+1)/2 , n+1] =
 *            1 if n is even
 * Range:     u8 n <= 10199, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9679 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t limit = n / 2;                /* maximum allowed q */
    if (limit < 2)                      /* no prime q ≥ 2 can be used */
        return 0;

    for (idx_t q = 2; q <= limit; ++q) {
        if (!is_prime(q))
            continue;                   /* q must be prime */

        /* compute p according to the parity of n */
        idx_t p = (n & 1ULL) ? (n - q - q) : (n - q);

        if (p < 3)                       /* p must be an odd prime ≥3 */
            continue;
        if (!is_prime(p))
            continue;

        /* Jacobi symbol condition */
        if (n & 1ULL) {                  /* n is odd */
            if (jacobi_symbol(q, n) != 1)
                continue;
        } else {                          /* n is even */
            idx_t r = (q + 1) / 2;
            if (jacobi_symbol(r, n + 1) != 1)
                continue;
        }

        ++count;                         /* a valid pair found */
    }
    return count;
}
