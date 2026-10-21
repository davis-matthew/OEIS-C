/*
 * A070295  Smallest composite number such that there are exactly n primes
 *          less than it (and coprime to it) which do not divide it, or 0 if
 *          no such number exists.
 * https://oeis.org/A070295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite m such that π(m–1) – ω(m) = n, where
 *            π(x) is the number of primes ≤ x and ω(m) the number of distinct
 *            prime divisors of m. If no such m exists a(n) = 0
 * Range:     u8 n <= 51, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *best = NULL;  /* best[n] = minimal composite with count n (0 = none) */
static size_t best_size = 0;
static const uint64_t LIMIT = 2000000ULL;  /* enough for n up to ≈150 000 */

static void init_precomp(void)
{
    if (best != NULL) return;               /* already built */

    /* Smallest prime factor array */
    uint64_t *spf = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= LIMIT; ++i) spf[i] = i;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* Prime counting π(x) */
    uint64_t *pi = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    pi[0] = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i) {
        pi[i] = pi[i-1] + ((i >= 2 && spf[i] == i) ? 1 : 0);
    }

    /* Allocate result array (index 0 unused, 0 indicates “not found”) */
    best = (seq_t *)calloc(LIMIT + 1, sizeof(seq_t));
    best_size = LIMIT + 1;

    /* Scan all composites up to LIMIT */
    for (uint64_t m = 4; m <= LIMIT; ++m) {
        if (spf[m] == m) continue;          /* skip primes */

        /* count distinct prime factors ω(m) */
        uint64_t x = m, last = 0, omega = 0;
        while (x > 1) {
            uint64_t p = spf[x];
            if (p != last) {
                ++omega;
                last = p;
            }
            x /= p;
        }

        uint64_t n_val = pi[m-1] - omega;    /* count of primes < m not dividing m */
        if (n_val >= 1 && n_val < best_size) {
            seq_t val = (seq_t)m;
            if (best[n_val] == (seq_t)0 || val < best[n_val])
                best[n_val] = val;
        }
    }

    free(spf);
    free(pi);
}

seq_t a(idx_t n)
{
    init_precomp();
    if ((idx_t)best_size <= n) return (seq_t)0;
    seq_t res = best[n];
    return (res == (seq_t)0) ? (seq_t)0 : res;
}
