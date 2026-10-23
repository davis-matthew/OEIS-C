/*
 * A132636  a(n) = Fibonacci(n) mod n^3.
 * https://oeis.org/A132636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) mod n^3
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 1629, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Perform multiplication modulo mod safely.
   For 64‑bit seq_t we use __int128 to avoid overflow,
   otherwise we rely on the normal multiplication which is safe
   for arbitrary‑precision seq_t types. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return ((unsigned __int128)a * b) % mod;
    } else {
        return (a * b) % mod;
    }
}

/* Compute (F(n), F(n+1)) modulo mod via fast doubling. */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }

    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);

    seq_t two_b = mul_mod(b, (seq_t)2, mod);
    seq_t t2    = (two_b + mod - a) % mod;          /* (2*b - a) mod mod */
    seq_t c     = mul_mod(a, t2, mod);              /* F(2k)   */
    seq_t aa    = mul_mod(a, a, mod);
    seq_t bb    = mul_mod(b, b, mod);
    seq_t d     = (aa + bb) % mod;                  /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;

    /* Compute n^3 safely.*/
    seq_t mod;
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        mod = (seq_t)((unsigned __int128)ns * ns * ns);
    } else {
        mod = ns * ns * ns;
    }

    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;               /* already reduced modulo n^3 */
}
