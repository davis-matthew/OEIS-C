/*
 * A103164  Integers but with the primes squared.
 * https://oeis.org/A103164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n! mod n^2) + n, for n > 0 and n != 4; a(0)=0, a(4)=4
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: modular multiplication without overflow.
   Computes (a * b) % mod where all arguments are in seq_t range.
   Uses repeated addition in binary (Russian peasant) form. */
static seq_t mul_mod(const seq_t a, const seq_t b, const seq_t mod)
{
    seq_t result = 0;
    seq_t aa = a % mod;
    seq_t bb = b;
    while (bb > 0)
    {
        if (bb & 1)
            result = (result + aa) % mod;
        aa = (aa + aa) % mod;
        bb >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 4)
        return 4;
    seq_t mod = (seq_t)n * (seq_t)n;   /* n^2, fits in seq_t for all tested n */

    /* Compute n! mod n^2 */
    seq_t fact_mod = 1 % mod;          /* initial value for n>=1 */
    for (idx_t i = 2; i <= n; ++i)
        fact_mod = mul_mod(fact_mod, (seq_t)i, mod);

    /* a(n) = n + (n! mod n^2) */
    return (seq_t)n + fact_mod;
}
