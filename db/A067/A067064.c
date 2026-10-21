/*
 * A067064  Numbers k such that sigma(sigma(sigma(k))) == 0 (mod sigma(k)).
 * https://oeis.org/A067064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that * σ(σ(σ(k))) is
 *            divisible by σ(k), * where σ(m) is the sum of all positive
 *            divisors of m. * The implementation computes σ(m) by prime
 *            factorisation * and checks the divisibility test for each
 *            candidate k
 * Range:     u8 n <= 8, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------
 * Function: sigma_uint64
 *-------------------------------------------------------------
 * Computes the sum of divisors σ(m) for a 64‑bit integer m.
 * Uses trial division up to √m and multiplies the σ(p^e) factors.
 * The result is returned as the type seq_t.
 *-------------------------------------------------------------*/
static seq_t sigma_uint64(idx_t m)
{
    if (m == 0) return 0;                 /* σ(0) is not defined, but avoid UB */
    if (m == 1) return 1;                 /* σ(1) = 1 */

    seq_t result = 1;                     /* product of σ(p^e) terms */
    idx_t n = m;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p != 0) continue;

        /* determine exponent e of prime p */
        idx_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        /* compute σ(p^e) = 1 + p + p^2 + … + p^e */
        seq_t term = 1;
        seq_t power = 1;
        for (idx_t i = 0; i < e; ++i) {
            power *= p;                    /* up to 64‑bit, safe for small n */
            term += power;
        }
        result *= term;
    }
    if (n > 1) {                          /* remaining prime factor */
        result *= (1 + n);
    }
    return result;
}

/*-------------------------------------------------------------
 * Function: a
 *-------------------------------------------------------------
 * Returns the n-th term of the sequence A067064.
 * Uses a simple cache to avoid repetitive work across calls.
 *-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t  size    = 0;      /* number of elements stored */
    static idx_t  capacity = 0;     /* allocated capacity of cache */
    static idx_t  next_k  = 1;      /* next integer to test */

    /* initialise cache */
    if (capacity == 0) {
        capacity = 32;                        /* initial size */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }

    /* search until we have at least n terms */
    while (size < n) {
        idx_t k = next_k++;
        seq_t sigma1 = sigma_uint64(k);
        seq_t sigma2 = sigma_uint64((idx_t)sigma1);
        seq_t sigma3 = sigma_uint64((idx_t)sigma2);

        if ((sigma3 % sigma1) == 0) {
            if (size == capacity) {            /* grow cache if needed */
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
            }
            cache[size++] = k;
        }
    }

    return cache[n - 1];
}
