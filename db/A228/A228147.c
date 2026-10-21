/*
 * A228147  Triangle T(n,k), read by rows: T(n,k) is the denominator of
 *          (1+2^(n-k+1))/(1-2^(k+1)).
 * https://oeis.org/A228147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the denominator of (1+2^(r-k+1))/(1-2^(k+1)) in lowest
 *            terms, where r and k are the row and column (both 0‑based) of
 *            the flat index n in the triangle read by rows
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 70, u64 n <= 2143,
 *            u128 n <= 5150, big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Find the 0‑based row number r for a given flat index n.
   Uses binary search on the triangular numbers. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* Find an upper bound for r. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Modular exponentiation for 64‑bit (or smaller) seq_t values.
   Computes base^exp mod mod. */
static seq_t mod_pow_64(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t result = 1 % mod;
    seq_t cur = base % mod;
    while (exp > 0) {
        if (exp & 1U)
            result = (unsigned __int128)result * cur % mod;
        cur = (unsigned __int128)cur * cur % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine (r,k) from the flat index n. */
    idx_t r = find_row(n);
    uint64_t before = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - before;

    /* Exponents for the numerator and denominator. */
    idx_t p = r - k + 1;   /* exponent in 1+2^p   */
    idx_t q = k + 1;       /* exponent in 2^q-1   */

    if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller build */
        seq_t denom;
        if (q == 64)
            denom = ~(seq_t)0;                 /* 2^64‑1 as all‑ones */
        else
            denom = ((seq_t)1 << q) - 1;       /* 2^q‑1 */

        /* (1+2^p) mod denom */
        seq_t numer_mod = (mod_pow_64((seq_t)2, p, denom) + 1) % denom;
        seq_t g = gcd(numer_mod, denom);
        return denom / g;
    } else { /* arbitrary‑precision build */
        seq_t denom = big_pow((seq_t)2, (seq_t)q);
        denom = denom - (seq_t)1;                /* 2^q‑1 */
        seq_t numer = big_pow((seq_t)2, (seq_t)p);
        numer = numer + (seq_t)1;                /* 1+2^p */
        seq_t g = gcd(numer, denom);
        return denom / g;
    }
}
