/*
 * A233284  a(n) = largest m such that 1, 2, ..., m divide n-th Fibonacci
 *          number; a(n) = A055874(A000045(n)).
 * https://oeis.org/A233284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9831, u16 n <= 9831, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A233284:  a(n) = largest m such that 1,2,…,m all divide F(n),
   where F(n) is the n-th Fibonacci number (F(1)=1, F(2)=1).  */
#include <stdint.h>
#include <stddef.h>

/* Compute F(n) mod mod for n≥1 and mod≥1 using fast doubling.
   The result fits into unsigned 64‑bit, since mod is small.  */
static inline uint64_t fib_mod_u64(idx_t n, idx_t mod)
{
    if (mod == 1) return 0;          /* any number ≡0 (mod 1) */
    if (n == 0) return 0;
    if (n == 1) return 1 % mod;

    uint64_t a = 0, b = 1;          /* a = F(k), b = F(k+1)  */
    /* mask points at the highest set bit of n   */
    idx_t mask = ((idx_t)1) << (63 - __builtin_clzll(n));

    while (mask) {
        /* multiply modulo 'mod' using 128‑bit intermediate to avoid overflow */
        uint64_t t   = ((b << 1) % mod + mod - a % mod) % mod;          /* 2*b - a (mod) */
        uint64_t c   = (__uint128_t)a * t % mod;                         /* F(2k) */
        uint64_t d   = (( (__uint128_t)a * a ) + ( (__uint128_t)b * b )) % mod; /* F(2k+1) */

        if (n & mask) {                     /* move to k->2k+1 */
            a = d;
            b = (c + d) % mod;
        } else {                            /* move to k->2k   */
            a = c;
            b = d;
        }
        mask >>= 1;
    }
    return a % mod;
}

/* Compute a(n) for n>=1 as defined in OEIS A233284. */
seq_t a(idx_t n)
{
    for (idx_t m = 1;; ++m) {
        /* For m=1 the remainder is always 0, so we may skip the test,
           but the generic test handles it correctly. */
        if (fib_mod_u64(n, m) != 0) {
            return (seq_t)(m - 1);   /* first m that fails gives the largest m-1 that satisfies  */
        }
    }
}
