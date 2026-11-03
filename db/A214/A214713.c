/*
 * A214713  a(n) is the least m > 0 such that Fibonacci(n+1)-m and n-m are
 *          relatively prime.
 * https://oeis.org/A214713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 350, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 828 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A function for OEIS sequence A214713: a(n) is the least m > 0 such that
 * Fibonacci(n+1)-m and n-m are relatively prime.
 * The implementation uses a fast‑doubling calculation of Fibonacci
 * modulo |n-m| so that it works for any seq_t (including 64‑bit or
 * arbitrary‑precision types) without overflow.  For each candidate m
 * it tests the coprimality condition and returns the minimal m. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t fibmod_u64(idx_t n, idx_t mod)
{
    /* Computes Fib(n) modulo mod.  Works for mod <= 10000,
     * so all intermediate products fit in 64 bits.  Uses the
     * fast doubling method iteratively. */
    if (mod == 1) return 0;
    seq_t a = 0;          /* F(0) */
    seq_t b = 1;          /* F(1) */
    /* Find position of highest set bit of n */
    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i) {
        /* c = F(2k) = F(k) * (2*F(k+1) - F(k)) */
        seq_t two_b = (b << 1) % mod;
        seq_t two_b_minus_a = (two_b + mod - a) % mod;
        seq_t c = (a * two_b_minus_a) % mod;
        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
        seq_t d = (a * a + b * b) % mod;
        if (((n >> i) & 1) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = (c + d) % mod;
        }
    }
    return a;
}
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is the sequence index, starting from 1. */
    for (idx_t m = 1;; ++m) {
        idx_t d = (idx_t)n - m;
        /* Skip the case d == 0; it never yields a valid answer
         * (it would require Fib(n+1)-n == ±1, which does not occur). */
        if (d == 0) continue;
        idx_t mod = (d > 0) ? d : (idx_t)(- (int64_t)d);   /* |n-m| <= n <= 10000 */
        seq_t fib_mod = fibmod_u64(n + 1, mod);            /* Fib(n+1) mod |d| */
        seq_t m_mod = (seq_t)m % (seq_t)mod;
        seq_t diff = (fib_mod + (seq_t)mod - m_mod) % (seq_t)mod;   /* (Fib(n+1)-m) mod |d| */
        if (gcd(diff, (seq_t)mod) == (seq_t)1)
            return (seq_t)m;
    }
}
