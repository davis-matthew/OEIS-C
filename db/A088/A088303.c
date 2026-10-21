/*
 * A088303  Smallest integer value of n!/(m_1!*m_2!*...*m_k!), where 1=m_1 <
 *          m_2 < ... is the sequence of integers coprime to n.
 * https://oeis.org/A088303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer value of n!/(m_1!*m_2!*...*m_k!), where
 *            1 = m_1 < m_2 < … is the sequence of integers coprime to n
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 17, u64 n <= 36, u128 n <= 56,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_mul.h"     /* seq_mul(seq_t, seq_t) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t, idx_t) */

/* ---------- global data (cached for all calls) ---------- */
static uint32_t *primes = NULL;       /* array of primes <= current_max */
static size_t   primes_cnt = 0;       /* number of primes */
static idx_t   *spf   = NULL;         /* smallest prime factor for each number <= current_max */
static idx_t    current_max = 0;       /* largest n that has been initialized */
static uint32_t *prime_index = NULL;  /* prime_index[p] = index of prime p in primes[], 0=not prime */

/* ---------- utilities ---------- */

/* Euclidean gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Legendre exponent of prime p in n! */
static inline idx_t legendre(idx_t n, idx_t p)
{
    idx_t sum = 0;
    idx_t div = p;
    while (div <= n) {
        sum += n / div;
        /* avoid overflow */
        if (div > n / p) break;
        div *= p;
    }
    return sum;
}

/* Build primes, spf and prime_index up to limit (inclusive). */
static void build_up_to(idx_t limit)
{
    /* free previous data */
    if (primes) free(primes);
    if (spf)   free(spf);
    if (prime_index) free(prime_index);

    /* Allocate spf array (size limit+1). */
    spf = (idx_t*)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!spf) return;

    /* spf[i] will be smallest prime factor of i */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((i * i) <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* gather primes into dynamic array */
    size_t est = (size_t)(limit / 2);    /* rough estimate */
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!primes) return;
    primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i) {
            if (primes_cnt == est) {      /* grow array if needed */
                est += est / 2 + 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
                if (!tmp) return;         /* allocation failure */
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }

    /* prime_index map */
    prime_index = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_index[primes[i]] = (uint32_t)(i + 1);  /* store i+1 to distinguish 0 */
}

/* Ensure data is available up to n. */
static void ensure_up_to(idx_t n)
{
    if (n <= current_max) return;
    build_up_to(n);
    current_max = n;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    ensure_up_to(n);

    /* exponent vector for n!  (size primes_cnt) */
    idx_t *exp = (idx_t*)calloc(primes_cnt, sizeof(idx_t));
    if (!exp) return (seq_t)0;

    for (size_t i = 0; i < primes_cnt; ++i)
        exp[i] = legendre(n, (idx_t)primes[i]);

    /* cumulative exponent vector of factorials of processed coprimes */
    idx_t *cum = (idx_t*)calloc(primes_cnt, sizeof(idx_t));
    if (!cum) {
        free(exp);
        return (seq_t)0;
    }

    for (idx_t m = 1; m <= n; ++m) {
        /* update cum with m! */
        if (m > 1) {
            idx_t x = m;
            while (x > 1) {
                idx_t p = spf[x];
                idx_t cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                size_t idx = (size_t)(prime_index[(uint32_t)p] - 1);
                cum[idx] += cnt;
            }
        }

        /* if m coprime to n, try to subtract cum from exp */
        if (gcd_idx(m, n) == 1) {
            bool ok = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                if (cum[i] > exp[i]) { ok = false; break; }
            }
            if (ok) {
                for (size_t i = 0; i < primes_cnt; ++i)
                    exp[i] -= cum[i];
            }
        }
    }

    /* reconstruct result from remaining exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (exp[i] == 0) continue;
        seq_t part = seq_pow((seq_t)primes[i], exp[i]);          /* p ^ exp */
        result = seq_mul(result, part);
    }

    free(exp);
    free(cum);
    return result;
}
