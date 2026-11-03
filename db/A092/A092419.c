/*
 * A092419  Let k = n-th nonsquare = A000037(n); then a(n) = smallest prime p
 *          such that the Kronecker-Jacobi symbol K(k,p) = -1.
 * https://oeis.org/A092419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9974 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = smallest prime p such that Kronecker symbol (k|p) = -1,
   where k is the nth positive nonsquare (A000037). */

/* nth_nonsquare: returns the nth positive nonsquare (1-based).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

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
static uint64_t nth_nonsquare(idx_t n)
{
    uint64_t k = 1;        /* current integer */
    uint64_t cnt = 0;      /* how many nonsquares seen */

    while (cnt < n)
    {
        ++k;
        uint64_t r = isqrt_u64(k);
        if (r * r != k)   /* k is not a perfect square */
            ++cnt;
    }
    return k;
}

/* Auxiliary: test whether an odd prime p satisfies (k|p) = -1.
   For p = 2 the Kronecker symbol is -1 iff k is odd and k ≡ ±3 (mod 8). */
seq_t a(idx_t n)
{
    uint64_t k = nth_nonsquare((uint64_t)n);

    for (uint64_t p = 2;; ++p)
    {
        if (!is_prime_u64(p))
            continue;                     /* skip non‑primes */

        if (p == 2)
        {
            /* (k|2) = 0 if k even; = -1 if k ≡ 3 or 5 (mod 8). */
            if ((k & 1ull) && ((k & 7ull) == 3u || (k & 7ull) == 5u))
                return (seq_t)p;
            continue;
        }

        if (k % p == 0)
            continue;                     /* (k|p) = 0 */

        if (jacobi_symbol(k, p) == -1)
            return (seq_t)p;
    }
}
