/*
 * A162820  Positive numbers n such that 60*n/(60+n) is an integer.
 * https://oeis.org/A162820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) are d - 60 where d is a divisor of 3600 and d > 60
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes all values once and then serves requests
 * from the cached list.  It works for any seq_t/idx_t provided by the
 * build system. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of helper functions */
static void gen_divisors(int idx, uint64_t current,
                         const int primes[], const int exps[],
                         int num_factors,
                         uint64_t *out, size_t *cnt);
static int cmp_uint64(const void *a, const void *b);

/* Cache for the computed n-values (d - 60).  There are at most
 * 45 divisors of 3600, so 45 entries are sufficient. */
static uint64_t nlist_cache[45];
static size_t nlist_cache_len = 0;
static bool nlist_inited = false;

/* Recursively generate all divisors of 3600. */
static void gen_divisors(int idx, uint64_t current,
                         const int primes[], const int exps[],
                         int num_factors,
                         uint64_t *out, size_t *cnt)
{
    if (idx == num_factors) {
        out[(*cnt)++] = current;
        return;
    }
    uint64_t val = current;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, primes, exps, num_factors, out, cnt);
        val *= (uint64_t)primes[idx];
    }
}

/* Comparator for qsort on uint64_t array. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Initialize the cache of sequence values. */
static void init_nlist(void)
{
    if (nlist_inited)
        return;

    const int primes[3] = {2, 3, 5};
    const int exps[3]  = {4, 2, 2};
    uint64_t divisors[45];
    size_t dcnt = 0;

    /* Generate all 45 divisors of 3600. */
    gen_divisors(0, 1ULL, primes, exps, 3,
                 divisors, &dcnt);

    /* Extract n = d - 60 for divisors d > 60. */
    size_t cnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        if (d > 60ULL) {
            nlist_cache[cnt++] = d - 60ULL;
        }
    }

    /* Sort the resulting list. */
    qsort(nlist_cache, cnt, sizeof(uint64_t), cmp_uint64);

    nlist_cache_len = cnt;
    nlist_inited = true;
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1 */

    init_nlist();

    if (n > (idx_t)nlist_cache_len)
        return (seq_t)0;          /* sequence is finite */

    size_t i = (size_t)(n - 1ULL);
    return (seq_t)nlist_cache[i];
}
