/*
 * A209253  Number of ways to write 2n-1 as the sum of a Sophie Germain prime
 *          and a practical number.
 * https://oeis.org/A209253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of representations of 2n‑1 as a Sophie Germain prime plus
 *            a practical number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* static lazy data structures */
static idx_t spf_limit = 0;
static idx_t *spf = NULL;
static bool *practical = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* extend all tables up to at least 'limit' */
static void extend_data(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    /* free any previous allocations */
    free(spf);
    free(practical);
    free(prime_list);

    /* allocate SPF array (0 meaning uninitialized) */
    spf = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    practical = (bool *)calloc(limit + 1, sizeof(bool));
    prime_list = (uint32_t *)calloc(limit / 2 + 10, sizeof(uint32_t));

    /* sieve for smallest prime factors */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* build prime list */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i)
            prime_list[prime_cnt++] = (uint32_t)i;

    /* practical numbers (Margenstern's criterion) */
    practical[1] = true; /* 1 is practical */
    for (idx_t m = 2; m <= limit; ++m) {
        idx_t x = m;
        bool ok = true;
        unsigned long long prod_sigma = 1;  /* product of sigma(p^e) so far */
        bool first = true;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t exp = 0;
            while (x % p == 0) { x /= p; ++exp; }

            if (first) {
                if (p != 2) { ok = false; break; }
                first = false;
            } else {
                if (p > prod_sigma + 1) { ok = false; break; }
            }

            /* sigma(p^exp) */
            unsigned long long sigma = 1, cur = 1;
            for (idx_t k = 0; k < exp; ++k) {
                cur *= p;
                sigma += cur;
            }
            prod_sigma *= sigma;
        }
        practical[m] = ok;
    }

    spf_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t target = 2 * n - 1;                /* number to decompose */
    idx_t max_needed = target * 2 + 10;      /* ensure primes up to 2p+1 */
    if (max_needed < target + 1)
        max_needed = target + 1;
    extend_data(max_needed);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)prime_list[i];
        if (p > target)
            break;

        idx_t two_p_plus = 2 * p + 1;
        if (!is_prime(two_p_plus))
            continue;                         /* not a Sophie Germain prime */

        idx_t q = target - p;
        if (q == 0)
            continue;                         /* practical number must be ≥1 */
        if (q <= spf_limit && practical[q])
            ++count;
    }

    return count;
}
