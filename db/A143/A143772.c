/*
 * A143772  If m is the n-th composite, then a(n) = gcd(k + m/k), where k is
 *          over all divisors of m.
 * https://oeis.org/A143772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(k + m/k) for all divisors k of the n-th composite m
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes primes and composites up to a fixed
 * limit sufficient for n <= 10000.  It then factors the requested
 * composite and evaluates the required gcd.  The code uses only
 * integer arithmetic and the helper `gcd(seq_t, seq_t)` from
 * "../utils/gcd.h", so it compiles both as C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Static storage for prime and composite numbers */
static uint32_t *primes = NULL;   /* primes, up to a chosen limit */
static size_t primes_len = 0, primes_cap = 0;
static uint64_t *composites = NULL; /* composites, starting at 4 */
static size_t composites_len = 0, composites_cap = 0;
static bool data_init = false;

/* --- Sieve of Eratosthenes ------------------------------------------------- */
static void init_data(void)
{
    if (data_init)
        return;

    const idx_t LIMIT = 200000;  /* enough for the 10‑000th composite */
    bool *is_prime = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!is_prime)
        return;
    /* Mark all entries as prime except 0 and 1 */
    for (idx_t i = 2; i <= LIMIT; i++)
        is_prime[(size_t)i] = true;

    /* Sieve */
    for (idx_t i = 2; i * i <= LIMIT; i++)
        if (is_prime[(size_t)i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[(size_t)j] = false;

    /* Allocate initial storage */
    primes_cap = 2000;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return;
    }
    composites_cap = 2000;
    composites = (uint64_t *)malloc(composites_cap * sizeof(uint64_t));
    if (!composites) {
        free(is_prime);
        free(primes);
        return;
    }

    /* Fill the arrays */
    for (idx_t i = 2; i <= LIMIT; i++) {
        if (is_prime[(size_t)i]) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(
                    primes, primes_cap * sizeof(uint32_t));
                if (!tmp)
                    break;
                primes = tmp;
            }
            primes[primes_len++] = (uint32_t)i;
        } else if (i >= 4) {   /* composite numbers start at 4 */
            if (composites_len == composites_cap) {
                composites_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(
                    composites, composites_cap * sizeof(uint64_t));
                if (!tmp)
                    break;
                composites = tmp;
            }
            composites[composites_len++] = (uint64_t)i;
        }
    }

    free(is_prime);
    data_init = true;
}

/* --- Factorisation and divisor generation --------------------------------- */
static seq_t gcd_of_divisor_sums(uint64_t m)
{
    /* Factor m using the pre‑computed primes */
    uint64_t factor[32];
    uint32_t exp[32];
    size_t fcnt = 0;
    uint64_t rem = m;
    for (size_t i = 0; i < primes_len &&
             (uint64_t)primes[i] * (uint64_t)primes[i] <= rem; i++) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            factor[fcnt] = p;
            exp[fcnt] = e;
            fcnt++;
        }
    }
    if (rem > 1) {
        factor[fcnt] = rem;
        exp[fcnt] = 1;
        fcnt++;
    }

    /* Generate all divisors */
    uint64_t divs[1024];   /* sufficient for m <= 200000 */
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fcnt; i++) {
        uint64_t p = factor[i];
        uint32_t e = exp[i];
        size_t cur = dcnt;
        uint64_t mul = 1;
        for (uint32_t k = 1; k <= e; k++) {
            mul *= p;
            for (size_t j = 0; j < cur; j++) {
                divs[dcnt++] = divs[j] * mul;
            }
        }
    }

    /* Compute gcd of all sums k + m/k */
    seq_t g = 0;
    for (size_t i = 0; i < dcnt; i++) {
        uint64_t k = divs[i];
        seq_t sum = (seq_t)k + (seq_t)(m / k);
        g = (g == 0) ? sum : gcd(g, sum);
    }
    return g;
}

/* --- Main sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset 1; n=0 not used */

    init_data();

    if (n > composites_len)
        return 0;              /* safety, should not happen for n<=10000 */

    uint64_t m = composites[n - 1];   /* n-th composite */
    return gcd_of_divisor_sums(m);
}
