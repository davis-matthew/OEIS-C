/*
 * A096396  a(n) = #{ 0 <= i <= n: K(n, i) = +1 } where K(n, i) is the
 *          Kronecker symbol.
 * https://oeis.org/A096396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 0 <= i <= n : K(n,i) = +1 } (Kronecker symbol)
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9136 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the Kronecker symbol (n | p) for a prime divisor p of i
 * n is the first argument of the symbol, p > 1 is a prime (not 2).
 * For p == 2 special cases are handled separately.
 */
static int kronecker_prime(idx_t n, idx_t p)
{
    if (p == 2) {
        /* (n | 2) : n even -> 0; odd -> ±1 according to n mod 8 */
        if ((n & 1U) == 0)
            return 0;
        idx_t r = n & 7U;        /* n mod 8 */
        return (r == 1U || r == 7U) ? 1 : -1;
    }

    /* odd prime: use Euler's criterion (n | p) = n^((p-1)/2) mod p  */
    if ((n % p) == 0)          /* n divisible by p -> symbol 0 */
        return 0;
    idx_t exp = (p - 1) >> 1;
    idx_t base = n % p;
    idx_t mod  = p;
    idx_t result = 1;

    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    return (result == mod - 1) ? -1 : (int)result;  /* ±1 */
}

/*-----
//  The following helpers are minimal and compile under C11 / C++17.
//  We use a small sieve of smallest prime factors (SPF) up to the
//  largest n that has been requested.  SPF is expanded lazily.
*/

static idx_t *spf = NULL;      /* SPF array, 1‑based */
static idx_t spf_size = 0;     /* current allocated size */

/* Allocate / grow the SPF array so that it covers at least [0..n]. */
static void prepare_spf(idx_t n)
{
    if (n + 1 <= spf_size)
        return;                 /* already sufficient */
    if (spf)
        free(spf);
    spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    spf_size = n + 1;
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {          /* i is prime */
            spf[i] = i;
            if (i <= n / i) {  /* avoid overflow */
                for (idx_t j = i * i; j <= n; j += i)
                    if (!spf[j])
                        spf[j] = i;
            }
        }
    }
}

/* Compute the Kronecker symbol K(n,i) for i >= 0.
 * For i == 0 we return 1 only when |n| == 1, otherwise 0.
 */
static int kronecker_symbol(idx_t n, idx_t i)
{
    if (i == 0)
        return (n == 1U) ? 1 : 0;      /* (n | 0) = 1 if |n|=1 else 0 */

    int res = 1;
    idx_t x = i;

    while (x != 1U) {
        idx_t p = spf[x];
        /* Count multiplicity of p in i */
        int e = 0;
        while ((x % p) == 0U) {
            x /= p;
            ++e;
        }
        /* If p divides n, symbol is 0 regardless of exponent */
        if ((n % p) == 0U)
            return 0;
        /* Only odd exponent contributes a sign change (±1) */
        if ((e & 1U) == 1) {
            int s = kronecker_prime(n, p);
            if (s == 0)        /* should not happen due to earlier check, but keep safe */
                return 0;
            res *= s;
        }
    }
    return res;        /* +1 or -1 */
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    prepare_spf(n);          /* ensure we can factor all numbers up to n */

    seq_t count = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (kronecker_symbol(n, i) == 1)
            ++count;
    }
    return count;
}
