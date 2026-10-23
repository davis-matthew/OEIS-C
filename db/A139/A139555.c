/*
 * A139555  a(n) = number of prime-powers (including 1) that each are <= n and
 *          are coprime to n.
 * https://oeis.org/A139555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{p prime ≤ n, p∤n} ⌊log_p n⌋ where log_p n is the
 *            largest exponent e with p^e ≤ n. This counts the prime‑powers ≤
 *            n that are coprime to n
 * Range:     u8 n <= 247, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/* Helper: simple sieve to generate primes up to limit n.
   Returns an array of primes and the count via *cnt_out. */
static void simple_sieve(idx_t n, uint64_t **primes_out, size_t *cnt_out)
{
    /* allocate boolean array */
    bool *is_prime = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    /* initially mark all as prime */
    for (idx_t i = 2; i <= n; ++i) is_prime[(size_t)i] = true;

    for (idx_t i = 2; (idx_t)((idx_t)i * (idx_t)i) <= n; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[(size_t)j] = false;
            }
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[(size_t)i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[(size_t)i]) {
        primes[idx++] = (uint64_t)i;
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out = cnt;
}

seq_t a(idx_t n)
{
    /* Always include 1 as a prime‑power coprime with n. */
    seq_t res = (seq_t)1;

    if (n < 2) return res;   /* n = 1 → only 1 */

    /* Generate all primes up to n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    simple_sieve(n, &primes, &primes_cnt);
    if (!primes) return res; /* allocation failure – return just 1 */

    seq_t seq_n = (seq_t)n;    /* cast once for comparisons */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        idx_t p_idx = (idx_t)p;
        /* Skip primes that divide n */
        if (n % p_idx == 0) continue;

        seq_t t = (seq_t)p;
        /* Count powers of p up to n */
        while (t <= seq_n) {
            ++res;
            if (t > seq_n / (seq_t)p) break;   /* avoid overflow */
            t *= (seq_t)p;
        }
    }

    free(primes);
    return res;
}
