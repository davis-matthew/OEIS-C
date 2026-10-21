/*
 * A061836  a(n) = smallest k>0 such that k+n divides k!.
 * https://oeis.org/A061836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that (k+n) divides k!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*==================================================================
  Prime sieve: build a small list of primes once (static).
==================================================================*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define PRIME_LIMIT 200000  /* large enough for all n up to ≈10 000 */

static uint32_t prime_list[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0)
        return;                     /* already initialized */

    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp)
        exit(1);                    /* allocation failed */

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[(uint32_t)j] = true;
        }
    }
    free(is_comp);
}

/*==================================================================
  Legendre's formula: exponent of prime p in k!
==================================================================*/
static inline int legendre(idx_t k, idx_t p)
{
    int e = 0;
    for (idx_t d = p; d <= k; d *= p) {
        e += k / d;
        if (d > k / p)            /* prevent overflow of d *= p */
            break;
    }
    return e;
}

/*==================================================================
  Main sequence element a(n)
==================================================================*/
seq_t a(idx_t n)
{
    /* special case from the OEIS data */
    if (n == 0)
        return (seq_t)1;

    init_primes();                               /* primes ready */

    for (idx_t k = 1; ; ++k) {
        idx_t m = k + n;                         /* candidate divisor */

        idx_t tmp = m;
        bool ok = true;

        /* factorise m using the precomputed primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > tmp)
                break;

            if (tmp % p == 0) {
                int e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }

                if (legendre(k, (idx_t)p) < e) {
                    ok = false;
                    break;
                }
            }
        }

        /* remaining factor, if any */
        if (ok && tmp > 1) {                     /* tmp is a prime > sqrt(m) */
            if (tmp > k) {                       /* prime factor larger than k -> impossible */
                ok = false;
            } else {
                if (legendre(k, tmp) < 1)
                    ok = false;
            }
        }

        if (ok)
            return (seq_t)k;                     /* found the smallest k */
    }
}
