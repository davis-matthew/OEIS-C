/*
 * A130603  a(0)=0, a(n) = the n-th positive integer that is coprime to
 *          (a(n-1)+1).
 * https://oeis.org/A130603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n≥1, a(n) is the n‑th positive integer that is *
 *            coprime to (a(n-1)+1)
 * Range:     u8 n <= 12, u16 n <= 495, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a cached sequence and, for each step,
 * calculates the n‑th coprime to m by binary searching with an
 * inclusion–exclusion count of coprimes up to a given bound. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Sequence storage and prime cache. */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;   /* length of a_cache */
static idx_t  a_cache_n    = 0;   /* largest index computed */

static uint64_t *prime_list = NULL;  /* list of primes up to current limit */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;     /* highest prime generated */

/* Ensure a_cache has space for indices up to n. */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (a_cache == NULL) exit(1);      /* abort on allocation failure */
        a_cache[0]   = 0;
        a_cache_cap  = (size_t)(n + 1);
        a_cache_n    = 0;
    } else if (a_cache_cap <= (size_t)n) {
        a_cache = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (a_cache == NULL) exit(1);
        a_cache_cap = (size_t)(n + 1);
    }
}

/* Ensure that prime_list contains all primes ≤ limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit < limit) {
        if (prime_list != NULL) free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_cnt);
        prime_limit = limit;
    }
}

/* Factor m into `pr` and return the number of distinct prime factors.
 * The caller must provide enough space; we use a static buffer of
 * sufficient size (80 primes is more than enough for 2^64). */
static size_t factor_primes(seq_t m, uint64_t *pr_out, size_t max_pr)
{
    size_t count = 0;
    seq_t temp = m;
    seq_t sq = floor_sqrt(m);
    idx_t limit = (idx_t)sq;
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && count < max_pr; ++i) {
        uint64_t p = prime_list[i];
        if ((seq_t)p > temp) break;
        if (temp % (seq_t)p == 0) {
            pr_out[count++] = p;
            while (temp % (seq_t)p == 0)
                temp /= (seq_t)p;
        }
    }
    if (temp > 1 && count < max_pr)
        pr_out[count++] = (uint64_t)temp;
    return count;
}

/* Count integers 1 … X that are coprime to m using inclusion–exclusion. */
static seq_t count_coprime_upto(seq_t X, uint64_t *pr, size_t pcnt)
{
    seq_t cnt = X;
    if (pcnt == 0) return cnt;           /* all numbers are coprime to 1 */
    uint64_t subsets = 1ULL << pcnt;
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        seq_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                /* avoid overflow: if prod > X / pr[i], set prod > X */
                if (prod > X / pr[i]) { prod = X + 1; break; }
                prod *= pr[i];
                ++bits;
            }
        }
        if (prod > X) continue;
        seq_t div = X / prod;
        if (bits & 1) cnt -= div;
        else          cnt += div;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    ensure_a_cache(n);

    /* Compute terms up to n if needed. */
    for (idx_t k = a_cache_n + 1; k <= n; ++k) {
        seq_t prev = a_cache[k - 1];
        seq_t m = prev + 1;          /* value to be coprime to */

        /* Factor m to get its distinct prime divisors. */
        static uint64_t primes[32];
        size_t pcnt = factor_primes(m, primes, sizeof(primes)/sizeof(primes[0]));

        /* Binary search for the k‑th coprime to m. */
        seq_t low = 1;
        seq_t high = m * (seq_t)k;   /* upper bound; safe because φ(m)/m ≤ 1 */

        while (low < high) {
            seq_t mid = low + (high - low) / 2;
            seq_t cnt = count_coprime_upto(mid, primes, pcnt);
            if (cnt < k)
                low = mid + 1;
            else
                high = mid;
        }
        a_cache[k] = low;
    }

    a_cache_n = n;
    return a_cache[n];
}
