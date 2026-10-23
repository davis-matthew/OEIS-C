/*
 * A129453  An analog of Pascal's triangle based on A092287. T(n,k) =
 *          A092287(n)/(A092287(n-k)*A092287(k)), 0 <= k <= n.
 * https://oeis.org/A129453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T(n,k) =
 *            A092287(n)/(A092287(n-k)*A092287(k)), where A092287(n) =
 *            product_{i=1..n} product_{j=1..n} gcd(i,j). The triangle is
 *            flattened row‑wise starting from row 0
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 57, u64 n <= 111,
 *            u128 n <= 216, big any n
 * Helpers:   exp_gcd_product, flattened_to_row_col, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/exp_gcd_product.h"
#include "../utils/flattened_to_row_col.h"

/* ------------------------------------------------------------------ 
   Generate all primes <= limit using a simple sieve.
   The caller must free (*primes_out) after use.
-------------------------------------------------------------------*/
static void sieve_primes(idx_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    bool *is_prime = (bool*)malloc((size_t)(limit + 1));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = false; is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (primes) {
        size_t id = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) primes[id++] = (uint32_t)i;
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out     = cnt;
}

/* ------------------------------------------------------------------ 
   Compute T(r,k) for 0 ≤ k ≤ r using prime exponents
-------------------------------------------------------------------*/
static seq_t compute_T(idx_t r, idx_t k)
{
    if (k == 0 || k == r) return (seq_t)1;

    /* primes up to r */
    uint32_t *primes = NULL;
    size_t prime_count = 0;
    sieve_primes(r, &primes, &prime_count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        idx_t eR   = exp_gcd_product(r,     p);
        idx_t eK   = exp_gcd_product(k,     p);
        idx_t eRk  = exp_gcd_product(r - k, p);
        idx_t exp  = eR - eK - eRk;
        if (exp > 0) {
            seq_t mult = seq_pow((seq_t)p, (idx_t)exp);
            result *= mult;
        }
    }
    free(primes);
    return result;
}

/* ------------------------------------------------------------------ 
   Public function required by the test harness
-------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t r, c;
    flattened_to_row_col(n, &r, &c);
    return compute_T(r, c);
}
