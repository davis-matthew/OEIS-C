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
 * Helpers:   fibmod_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A function for OEIS sequence A214713: a(n) is the least m > 0 such that
 * Fibonacci(n+1)-m and n-m are relatively prime.
 * The implementation uses a fast‑doubling calculation of Fibonacci
 * modulo |n-m| so that it works for any seq_t (including 64‑bit or
 * arbitrary‑precision types) without overflow.  For each candidate m
 * it tests the coprimality condition and returns the minimal m. */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */
#include <stdbool.h>
#include "../utils/fibmod_u64.h"

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
