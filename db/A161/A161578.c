/*
 * A161578  a(1)=2. a(n) = the smallest integer > a(n-1) such that d(a(n)) <=
 *          d(n), where d(n) = the number of divisors of n.
 * https://oeis.org/A161578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = the smallest integer > a(n‑1) with d(a(n)) ≤
 *            d(n), * where d(x) is the number of divisors of x
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Automatic prime table – sufficient for all needed sqrt(n) (n ≤ 10⁴). */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;
    const size_t LIMIT = 1000;               /* sqrt(10⁶) > 1000 */
    bool *is_comp = (bool *)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i)
        is_comp[i] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* Count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            ++primes_cnt;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            primes[idx++] = (uint32_t)i;
    free(is_comp);
    primes_ready = true;
}

/* Count divisors of a 64‑bit integer using the prime table */
static uint64_t divisor_count_uint64(uint64_t n)
{
    if (n <= 1)
        return 1;
    uint64_t count = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p;
        if (pp * pp > tmp)
            break;
        if (tmp % pp == 0) {
            unsigned exp = 0;
            while (tmp % pp == 0) {
                tmp /= pp;
                ++exp;
            }
            count *= (uint64_t)(exp + 1);
        }
    }
    if (tmp > 1)                     /* remaining prime factor */
        count *= 2;
    return count;
}

/* Sequence cache: a[0] unused, a[1].. */
static seq_t *a_cache = NULL;
static size_t a_len  = 0;   /* highest index computed */
static size_t a_cap  = 0;   /* cache capacity */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primes are prepared */
    init_primes();

    /* Resize cache if necessary */
    if (a_cache == NULL) {
        a_cap = (size_t)n + 2;      /* at least n+1 indices */
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > a_cap) {
        a_cap = (size_t)n + 2;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }

    /* Compute terms up to n */
    while (a_len < n) {
        idx_t i = a_len + 1;        /* new index to compute */
        if (i == 1) {
            a_cache[1] = (seq_t)2;
        } else {
            seq_t prev = a_cache[i - 1];
            uint64_t d_i = divisor_count_uint64((uint64_t)i);

            /* Find next candidate */
            uint64_t cand = (uint64_t)prev + 1;
            while (divisor_count_uint64(cand) > d_i)
                ++cand;
            a_cache[i] = (seq_t)cand;
        }
        ++a_len;
    }

    return a_cache[n];
}
