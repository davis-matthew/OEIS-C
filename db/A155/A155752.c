/*
 * A155752  Where 2's occur in the prime differences A001223.
 * https://oeis.org/A155752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A029707(n)-1; positions of prime gaps equal to 2.
 *            Computed by sieving primes up to a growing limit and collecting
 *            gaps of 2
 * Range:     u8 n <= 52, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static storage for primes and twin‑gap indices */
static bool              initialized      = false;      /* first call flag   */
static uint64_t          current_limit    = 0;          /* sieve upper bound */
static uint64_t         *primes           = NULL;       /* list of primes    */
static size_t            primes_cap       = 0;          /* primes array cap  */
static size_t            primes_cnt       = 0;          /* number of primes  */
static uint64_t         *twin_indices     = NULL;       /* gap‑index‑1 values*/
static size_t            twin_cap         = 0;          /* twin array cap    */
static size_t            twin_cnt         = 0;          /* number of twins   */

/* floor square root for uint64_t */
static uint64_t floor_sqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* sieve and collect twin gaps */
static void recompute_primes_and_twins(uint64_t limit)
{
    /* dynamic sieve bit array */
    static uint8_t *is_comp = NULL;
    static size_t  is_comp_cap = 0;
    if (is_comp_cap <= limit) {
        free(is_comp);
        is_comp = (uint8_t*)malloc(limit + 1);
        is_comp_cap = limit + 1;
    }
    memset(is_comp, 0, limit + 1);
    if (limit >= 0) {
        is_comp[0] = 1;
        if (limit >= 1) is_comp[1] = 1;
    }

    /* Eratosthenes */
    uint64_t sqrt_limit = floor_sqrt_uint64(limit);
    for (uint64_t i = 2; i <= sqrt_limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* store primes */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++new_cnt;

    if (primes_cap < new_cnt) {
        free(primes);
        primes = (uint64_t*)malloc(new_cnt * sizeof(uint64_t));
        primes_cap = new_cnt;
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes[idx++] = i;
    primes_cnt = new_cnt;

    /* collect twin gaps */
    size_t new_twin_cnt = 0;
    for (size_t i = 1; i < primes_cnt; ++i) {
        if (primes[i] - primes[i - 1] == 2) {          /* gap == 2 */
            if (twin_cap <= new_twin_cnt) {
                size_t new_cap = twin_cap ? twin_cap * 2 : 16;
                twin_indices = (uint64_t*)realloc(twin_indices, new_cap * sizeof(uint64_t));
                twin_cap = new_cap;
            }
            twin_indices[new_twin_cnt++] = (uint64_t)(i - 1); /* store i-1 */
        }
    }
    twin_cnt = new_twin_cnt;
    current_limit = limit;
}

/* ensure enough twin indices */
static void ensure_up_to(idx_t need)
{
    if (!initialized) {
        recompute_primes_and_twins(20000);          /* initial moderate limit */
        initialized = true;
    }
    while (twin_cnt < (size_t)need) {
        uint64_t new_limit = current_limit * 2;
        if (new_limit == current_limit) new_limit = 40000; /* avoid zero */
        recompute_primes_and_twins(new_limit);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but never called */
    ensure_up_to(n);
    return (seq_t)twin_indices[n - 1];
}
