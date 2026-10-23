/*
 * A130480  a(n) = lcm(b(0),b(1),b(2),...,b(n)), where b(m) = A130479(m).
 * https://oeis.org/A130480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1!!, 2!!, 3!!, …, n!!)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where k!! is the double factorial of k. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* -------------------------------------------------------------
       1. Generate all primes up to n with a simple sieve.
       ------------------------------------------------------------- */
    idx_t limit = n;
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0; /* memory failure */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; (idx_t)(p * p) <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p)
                is_prime[j] = false;
        }
    }

    /* store primes in a dynamic array */
    idx_t *primes = (idx_t *)malloc((size_t)(limit / 2 + 1) * sizeof(idx_t));
    size_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[prime_count++] = i;

    /* -------------------------------------------------------------
       2. Pre‑compute v2(i!) for i up to n/2 to evaluate the 2‑adic
          exponent of an even double factorial quickly.
       ------------------------------------------------------------- */
    idx_t half = n / 2;
    uint32_t *v2_fact = (uint32_t *)malloc((size_t)(half + 1) * sizeof(uint32_t));
    v2_fact[0] = 0;
    for (idx_t i = 1; i <= half; ++i) {
        uint32_t cnt = 0;
        idx_t x = i;
        while ((x & 1) == 0) { /* count factors of 2 */
            ++cnt;
            x >>= 1;
        }
        v2_fact[i] = v2_fact[i - 1] + cnt;
    }

    /* -------------------------------------------------------------
       3. For each prime compute its maximal exponent inside any
          k!! (k ≤ n) and accumulate the product.
       ------------------------------------------------------------- */
    seq_t result = (seq_t)1;          /* a(0) = 1 */

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = primes[pi];
        uint32_t max_exp = 0;

        if (p == 2) {
            /* v2(k!!) = k/2 + v2((k/2)!) for even k */
            for (idx_t k = 2; k <= n; ++k) {
                if ((k & 1) == 0) {          /* even k */
                    idx_t m = k >> 1;
                    uint32_t e = (uint32_t)m + v2_fact[m];
                    if (e > max_exp)
                        max_exp = e;
                }
            }
        } else {
            /* v_p(k!!) depends on the parity of k */
            for (idx_t k = 2; k <= n; ++k) {
                uint32_t e = 0;
                if ((k & 1) == 0) {          /* even k : use floor(q/2) */
                    for (idx_t pw = p; pw <= k; pw *= p) {
                        idx_t q = k / pw;
                        e += (uint32_t)(q / 2);
                    }
                } else {                     /* odd k : use ceil(q/2) */
                    for (idx_t pw = p; pw <= k; pw *= p) {
                        idx_t q = k / pw;
                        e += (uint32_t)((q + 1) / 2);
                    }
                }
                if (e > max_exp)
                    max_exp = e;
            }
        }

        if (max_exp > 0) {
            seq_t pow_val = seq_pow((seq_t)p, (idx_t)max_exp);
            result = seq_mul(result, pow_val);
        }
    }

    /* free temporary memory */
    free(is_prime);
    free(primes);
    free(v2_fact);

    return result;
}
