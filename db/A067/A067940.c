/*
 * A067940  Let rep(n) be the n-th repunit number, sequence gives values of n
 *          such that rep(n)==rep(7) (mod n).
 * https://oeis.org/A067940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A067940: a(n) = the n-th positive integer m such that
   rep(m) ≡ rep(7) (mod m), where rep(k)=111…1 (k digits).  */
/* The condition is equivalent to 10^m ≡ 10^7 (mod 9m).  */

#include <stdint.h>
#include <stdbool.h>

/* Helper: fast exponentiation of 10 modulo a modulus (9*m).  */
/* Uses unsigned __int128 to avoid overflow when m is a 64‑bit integer. */
static unsigned __int128 modpow10(idx_t exp, unsigned __int128 mod)
{
    unsigned __int128 result = 1U;
    unsigned __int128 base   = 10U % mod;
    while (exp != 0) {
        if (exp & 1U)
            result = (result * base) % mod;
        base   = (base * base) % mod;
        exp >>= 1U;
    }
    return result;
}

/* Test whether m satisfies the defining congruence.  */
static bool satisfies(idx_t m)
{
    if (m == 0)
        return false;

    unsigned __int128 mod = (unsigned __int128)m * 9U;   /* modulus 9*m */
    if (mod == 0)   /* overflowed, never happens for m up to 64 bits */
        return false;

    /* 10^7 mod (9*m) */
    unsigned __int128 pow7 = modpow10(7U, mod);

    /* 10^m mod (9*m) */
    unsigned __int128 powm = modpow10(m, mod);

    return powm == pow7;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n < 1, return 0 safely */

    idx_t found = 0;         /* number of matching terms found so far */
    idx_t m = 0;             /* current integer being tested */

    while (found < n) {
        ++m;
        if (satisfies(m))
            ++found;
    }
    return (seq_t)m;
}
