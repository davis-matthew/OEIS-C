/*
 * A074215  Numbers m such that m and F(m) are relatively prime, where F(m)
 *          denotes the m-th Fibonacci number.
 * https://oeis.org/A074215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m ≥ 1 such that gcd(m, F(m)) = 1 (F(m)
 *            is the m-th Fibonacci number)
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_add, mod_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_add.h"
#include "../utils/mod_sub.h"

static inline seq_t mul_mod_small(seq_t a, seq_t b, seq_t mod)
{
    /* a, b, mod < mod <= 2^64-1, so 64‑bit multiplication is safe */
    uint64_t prod = ((uint64_t)a * (uint64_t)b) % (uint64_t)mod;
    return (seq_t)prod;
}

/* ---------- fast doubling Fibonacci modulo m ---------- */
static void fib_mod_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1) */

    seq_t two_b = mul_mod_small(b, 2, mod);
    seq_t a2b   = mod_sub(two_b, a, mod);      /* (2*b - a) mod mod */

    seq_t c = mul_mod_small(a, a2b, mod);                                 /* F(2k)   */
    seq_t d = mod_add(mul_mod_small(a, a, mod), mul_mod_small(b, b, mod), mod); /* F(2k+1) */

    if (n & 1) {
        *fn  = d;
        *fn1 = mod_add(c, d, mod);
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

static inline seq_t fib_mod(idx_t n, idx_t mod)
{
    seq_t f, f1;
    fib_mod_pair(n, (seq_t)mod, &f, &f1);
    return f;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t  m         = 1;          /* current integer to test   */
    idx_t  count     = 0;          /* how many terms found so far */
    seq_t  result    = 0;          /* the n-th wanted value      */

    while (count < n) {
        seq_t f_mod = fib_mod(m, m);          /* F(m) mod m             */
        seq_t m_seq = (seq_t)m;
        if (gcd(m_seq, f_mod) == 1) {
            ++count;
            if (count == n) result = m_seq;
        }
        ++m;
    }
    return result;
}
