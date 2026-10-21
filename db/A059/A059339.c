/*
 * A059339  a(n) is the number of representations of n as a sum x+y, such that
 *          bigomega(n) = bigomega(x)+bigomega(y) and 1 <= x <= y < n.
 * https://oeis.org/A059339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2474, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of representations of n as x+y with 1 ≤ x ≤ y < n and
   bigomega(n) = bigomega(x)+bigomega(y) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* smallest prime factor array (spf[0]=0, spf[1]=1) */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

/* bigomega array (Ω) */
static uint32_t *bigomega = NULL;
static size_t omega_size = 0;

/* list of primes found by the sieve */
static uint32_t *primes = NULL;
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;

/* cache of already computed a(n) */
static seq_t *cache = NULL;
static bool   *computed = NULL;
static size_t cache_size = 0;

/* extend all precomputed tables up to at least limit (inclusive) */
static void extend_precomp(uint32_t limit)
{
    if (limit + 1 <= spf_size)   /* already covers the range */
        return;

    size_t old_size = spf_size;
    size_t new_size = (size_t)limit + 1;

    /* --- spf and bigomega arrays --- */
    spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
    bigomega = (uint32_t *)realloc(bigomega, new_size * sizeof(uint32_t));
    if (!spf || !bigomega)
        exit(EXIT_FAILURE);
    /* zero new elements */
    memset(spf + old_size, 0, (new_size - old_size) * sizeof(uint32_t));
    memset(bigomega + old_size, 0, (new_size - old_size) * sizeof(uint32_t));

    /* initialize the two first entries */
    spf[0] = 0;
    spf[1] = 1;
    bigomega[0] = 0;
    bigomega[1] = 0;

    /* linear sieve to fill spf and primes */
    for (size_t i = 2; i < new_size; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || i * (size_t)p >= new_size)
                break;
            spf[i * p] = p;
        }
    }

    /* compute bigomega for the new range */
    for (size_t i = old_size; i < new_size; ++i) {
        if (i <= 1) {
            bigomega[i] = 0;
        } else {
            bigomega[i] = bigomega[i / spf[i]] + 1;
        }
    }
    spf_size = new_size;
    omega_size = new_size;

    /* --- cache arrays --- */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    computed = (bool *)realloc(computed, new_size * sizeof(bool));
    if (!cache || !computed)
        exit(EXIT_FAILURE);
    /* initialise new entries */
    for (size_t i = cache_size; i < new_size; ++i) {
        cache[i] = (seq_t)0;
        computed[i] = false;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    uint32_t ui_n = (uint32_t)n;          /* safe for n ≤ 100 000 */
    extend_precomp(ui_n);

    if (computed[n])
        return cache[n];

    uint32_t omega_n = bigomega[ui_n];
    seq_t count = (seq_t)0;

    /* iterate only over x ≤ y, i.e. x ≤ n/2 */
    uint32_t half = ui_n / 2;
    for (uint32_t x = 1; x <= half; ++x) {
        if (omega_n == bigomega[x] + bigomega[ui_n - x])
            ++count;
    }

    cache[n] = count;
    computed[n] = true;
    return count;
}
