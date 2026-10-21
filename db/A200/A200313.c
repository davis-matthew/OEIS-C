/*
 * A200313  E.g.f. satisfies: A(x) = exp(x^3*A(x)^3/3!).
 * https://oeis.org/A200313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)^(n-1) * (3*n)! / (n! * (3!)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Return the exponent of prime p in n! (Legendre's formula) */
static int legendre(idx_t n, int p)
{
    int sum = 0;
    for (idx_t m = n; m; m /= p)
        sum += (int)(m / p);
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Multiply by the (3*n+1)^(n-1) factor first */
    seq_t result = seq_pow((seq_t)(3 * n + 1), n - 1);

    /* Prime sieve up to 3*n for the factorial ratio part */
    idx_t limit = 3 * n;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return result;   /* out‑of‑memory: return what we have */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[(size_t)j] = false;

    for (int p = 2; p <= limit; ++p)
    {
        if (!is_prime[(size_t)p]) continue;

        int e = legendre(3 * n, p) - legendre(n, p);
        if (p == 2 || p == 3) e -= (int)n;          /* subtract (3!)^n factor */

        if (e > 0)
            result = seq_mul(result, seq_pow((seq_t)p, (idx_t)e));
    }

    free(is_prime);
    return result;
}
