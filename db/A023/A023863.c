/*
 * A023863  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2) and t = A002808 (composite numbers).
 * https://oeis.org/A023863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} i * t(n+1-i), where t(k) is the k-th
 *            composite number (4,6,8,9,10,12,...)
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for composite numbers and primes used for primality tests */
static seq_t *composites = NULL;
static size_t comp_len = 0, comp_cap = 0;

static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t num_checked = 3;      /* last number tested for compositeness */

/* Ensure that composites array has at least n entries (t(1)…t(n)) */
static void ensure_composites(idx_t n)
{
    if (comp_len >= (size_t)n)
        return;

    /* Initialize primes array with the first two primes 2,3 if needed */
    if (primes_len == 0) {
        primes_cap = 4;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes[1] = 3;
        primes_len = 2;
    }

    while (comp_len < (size_t)n) {
        num_checked++;          /* candidate number */

        bool is_prime = true;
        /* Trial division by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < primes_len; i++) {
            idx_t p = primes[i];
            if (p * p > num_checked)
                break;
            if (num_checked % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store prime for future tests */
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 4;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                 primes_cap * sizeof(uint64_t));
                primes = tmp;
            }
            primes[primes_len++] = num_checked;
        } else {
            /* Store composite */
            if (comp_len == comp_cap) {
                comp_cap = comp_cap ? comp_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(composites,
                               comp_cap * sizeof(seq_t));
                composites = tmp;
            }
            composites[comp_len++] = (seq_t)num_checked;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; undefined for n=0 */

    /* Make sure we have the necessary composites */
    ensure_composites(n);

    seq_t sum = 0;
    idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        idx_t idx = n + 1 - i;     /* composite index used (1-based) */
        sum += (seq_t)i * composites[idx - 1];
    }
    return sum;
}
