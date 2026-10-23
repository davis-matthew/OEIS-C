/*
 * A129108  a(0)=1; a(n) is the smallest positive integer such that
 *          lcm(a(n-1), a(n)) = n!.
 * https://oeis.org/A129108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p prime ≤ n, n mod p even} p^{e(p,n)}, where e(p,n) =
 *            ⌊n/p⌋ + ⌊n/p²⌋ + … (exponent of p in n!)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 43,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(idx_t n) returns the nth term of OEIS A129108.
    Uses a naive sieve to enumerate primes ≤ n for each call. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;          /* a(0) = 1 */

    /* Simple sieve up to n to obtain all primes ≤ n. */
    size_t limit = (size_t)n;
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) return (seq_t)0;           /* allocation failure – unlikely */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    seq_t result = (seq_t)1;

    for (size_t p = 2; p <= limit; ++p)
    {
        if (!is_prime[p]) continue;

        /* exponent of prime p in n! */
        idx_t e = 0;
        idx_t power = (idx_t)p;
        while (power <= n)
        {
            e += n / power;
            if (power > n / p) break;
            power *= p;
        }

        /* include this prime power iff n mod p is even */
        if ( ((n % p) & 1ULL) == 0 )
        {
            seq_t pow = seq_pow((seq_t)p, e);
            result = seq_mul(result, pow);
        }
    }

    free(is_prime);
    return result;
}
