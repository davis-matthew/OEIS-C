/*
 * A124201  a(n) = n-th positive integer from among those positive integers
 *          which are coprime to every noncomposite (1 or prime) which is < n
 *          and does not divide n.
 * https://oeis.org/A124201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer coprime to every prime p < n that
 *            does not * divide n. (The set of non‑composite numbers < n that
 *            do not divide n * consists solely of those primes.)
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;          /* array of primes */
static size_t    primes_cnt = 0;         /* number of primes in the array */
static idx_t     primes_max = 0;         /* maximum limit for which primes are built */

/* Ensure that primes for all numbers < limit are available. */
static void
ensure_primes(idx_t limit)
{
    if (primes == NULL || limit > primes_max) {
        if (primes != NULL) free(primes);
        build_primes_upto(limit, (uint64_t **)&primes, &primes_cnt);
        primes_max = limit;
    }
}

/* Cache for computed a(n) values.  0 means “not yet computed”. */
static seq_t *a_cache = NULL;
static idx_t  a_cache_size = 0;

static void
cache_ensure(idx_t n)
{
    if (n > a_cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        memcpy(new_cache, a_cache, a_cache_size * sizeof(seq_t));
        for (idx_t i = a_cache_size; i < new_size; ++i) new_cache[i] = 0;
        free(a_cache);
        a_cache = new_cache;
        a_cache_size = new_size;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n=0 */

    cache_ensure(n);
    if (a_cache[n - 1] != 0) return a_cache[n - 1];

    /* Build list of primes < n that do not divide n. */
    ensure_primes(n > 1 ? n - 1 : 1);
    size_t ip = 0;
    uint64_t temp_primes[2048];
    size_t temp_cnt = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] < n; ++i) {
        if (n % primes[i] != 0) {
            if (temp_cnt < sizeof(temp_primes) / sizeof(temp_primes[0]))
                temp_primes[temp_cnt++] = primes[i];
        }
    }

    idx_t candidate = 1;
    idx_t count = 0;
    while (true) {
        bool ok = true;
        for (size_t j = 0; j < temp_cnt; ++j) {
            if (candidate % temp_primes[j] == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            ++count;
            if (count == n) {
                seq_t result = (seq_t)candidate;
                a_cache[n - 1] = result;
                return result;
            }
        }
        ++candidate;
    }
}
