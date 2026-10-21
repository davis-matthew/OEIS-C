/*
 * A055204  Squarefree part of n!: n! divided by its largest square divisor.
 * https://oeis.org/A055204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes p ≤ n with odd exponent in n!
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 40, u64 n <= 70, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ∏_{p≤n, v_p(n!) odd} p          */
/* where v_p(n!) = ⌊n/p⌋ + ⌊n/p^2⌋ + … */

#include <stdlib.h>
#include <stdbool.h>

/* Computes a(n) = squarefree part of n! */
seq_t a(idx_t n)
{
    /* Static prime table (incremental sieve) */
    static idx_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap = 0;
    static idx_t max_prime_checked = 1;

    /* Lazy allocation of prime table */
    if (!primes) {
        prime_cap = 256;
        primes = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!primes) return 0;      /* allocation failure – return 0 as error */
    }

    /* Extend prime table up to n */
    if (n > max_prime_checked) {
        for (idx_t cand = max_prime_checked + 1; cand <= n; ++cand) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= cand; ++i) {
                if (cand % primes[i] == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                if (prime_count == prime_cap) {   /* grow table */
                    prime_cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(primes, prime_cap * sizeof(idx_t));
                    if (!tmp) return 0;          /* allocation failure */
                    primes = tmp;
                }
                primes[prime_count++] = cand;
            }
        }
        max_prime_checked = n;
    }

    /* Compute the squarefree part */
    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p > n) break;
        idx_t t = n;
        unsigned parity = 0;
        while (t) {
            t /= p;
            parity ^= (t & 1ul);
        }
        if (parity) {
            result *= p;
        }
    }
    return result;
}
