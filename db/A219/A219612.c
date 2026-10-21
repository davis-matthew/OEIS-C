/*
 * A219612  Numbers k that divide the sum of the first k Fibonacci numbers
 *          (beginning with F(0)).
 * https://oeis.org/A219612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that F(k+1) ≡ 1 (mod k) * (F(0)=0,
 *            F(1)=1). This implementation searches for k sequentially * and
 *            returns the nth such k. All intermediate calculations use * the
 *            natural integer type idx_t, which is wide enough for the *
 *            searched range. The result is cast to seq_t; i...
 * Range:     u8 n <= 50, u16 n <= 4921, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------------*/
/*  Fast‑doubling Fibonacci modulo a given modulus.                 */
/*------------------------------------------------------------------*/
static void fib_mod_pair(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    idx_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);

    /* t = 2*b - a  (mod mod) */
    idx_t t = ( (b << 1) % mod + mod - a ) % mod;

    /* c = a * t  (mod mod) */
    idx_t c = (a * t) % mod;

    /* d = a*a + b*b  (mod mod) */
    idx_t d = ( (a * a) % mod + (b * b) % mod ) % mod;

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/*------------------------------------------------------------------*/
/*  Test if k divides the sum of the first k Fibonacci numbers, i.e.
/*  whether F(k+1) ≡ 1 (mod k).                                       */
/*------------------------------------------------------------------*/
static bool is_good_k(idx_t k)
{
    if (k == 1)          /* 1 divides the empty sum 0 */
        return true;
    idx_t f, tmp;
    fib_mod_pair(k + 1, k, &f, &tmp);
    return f == 1;
}

/*------------------------------------------------------------------*/
/*  Public sequence function                                        */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, return 0 for safety */
        return (seq_t)0;

    idx_t found = 0;          /* count of successful k's so far */
    for (idx_t k = 1; found < n; ++k) {
        if (is_good_k(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
    /* Should never reach here */
    return (seq_t)0;
}
