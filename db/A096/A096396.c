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
 * Helpers:   kronecker_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/kronecker_prime.h"

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
