/*
 * A086382  k divides F(k*n^2+1)-F(k+1) for 1<=k<=a(n) where F(k) is the k-th
 *          Fibonacci number.
 * https://oeis.org/A086382
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086382:
   a(n) is the maximum k such that for all 1 ≤ j ≤ k,
   j divides F(j*n^2+1) – F(j+1),
   where F(m) is the m‑th Fibonacci number.
   The function evaluates the definition directly, using fast‑doubling
   to compute Fibonacci numbers modulo a small modulus. */

#include <stdint.h>
#include <stddef.h>

/* Fast‑doubling algorithm for Fibonacci numbers modulo 'mod'.
   Recursively computes F(n) and F(n+1) modulo 'mod'. */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = (mod == 1) ? 0 : 1 % mod;
        return;
    }

    seq_t a, b, t, c, d;
    fib_pair_mod(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1) */

    /* F(2k) = F(k) * [2*F(k+1) – F(k)] */
    t = ( ( (seq_t)2 * b % mod) + mod - a ) % mod;
    c = ( a * t ) % mod;                       /* F(2k)   */

    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    d = ( (a * a) % mod + (b * b) % mod ) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Return F(n) modulo 'mod'. */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    if (mod == 1)
        return 0;
    seq_t f, f1;
    fib_pair_mod(n, mod, &f, &f1);
    return f % mod;
}

seq_t a(idx_t n)
{
    /* Definition valid for n ≥ 2; behavior for smaller n is unspecified. */
    if (n < 2)
        return 0;

    idx_t k = 1;
    while (1) {
        seq_t mod = (seq_t)k;          /* modulus is the current k */

        /* Compute indices for Fibonacci numbers. */
        idx_t n_sq = n * n;            /* n²  (fits into idx_t for n ≤ 10⁵) */
        idx_t idx1 = k * n_sq + 1;     /* k*n² + 1  */
        seq_t f1 = fib_mod(idx1, mod);/* F(k*n²+1) mod k */
        seq_t f2 = fib_mod(k + 1, mod);/* F(k+1)    mod k */

        /* Check divisibility: k | (F(k*n²+1) – F(k+1)) */
        if ((f1 + mod - f2) % mod != 0)
            break;                    /* first failure ==> a(n)=k-1 */

        ++k;
    }
    return (seq_t)(k - 1);
}
