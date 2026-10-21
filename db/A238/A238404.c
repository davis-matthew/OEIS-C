/*
 * A238404  Number of ways a prime from A087054 can be decomposed as a sum of
 *          the form p*q+q*r+r*p where p, q and r are distinct primes (p < q <
 *          r).
 * https://oeis.org/A238404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n): number of representations of the n‑th prime of
 *  OEIS A087054 as p*q + q*r + r*p with distinct primes p < q < r.
 *  The implementation builds a cache of all representable primes up
 *  to an increasing limit until at least n terms have been found.
 *  Each cache rebuild recomputes the counts from scratch.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *term_cache  = NULL;      /* array of a(k) values       */
static size_t cache_len   = 0;          /* number of stored terms     */
static uint64_t search_limit = 200000ULL; /* current maximal value for primes */

/* ------------------------------------------------------------------ */
/* Recompute the cache for all primes ≤ limit.  It frees any old
 * cache.  The function allocates a sieve, the prime list and a
 * counter array, then enumerates all triples p<q<r generating prime
 * sums within the limit.  The result is stored in term_cache
 * containing the counts for each representable prime in ascending
 * order.  */
static void recompute_cache(uint64_t limit)
{
    /* ---- 1. Sieve for primes up to limit ---------------------------------- */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1));
    if (is_prime == NULL) return;              /* allocation error */

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* build list of primes */
    size_t prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    uint32_t *primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (primes == NULL) { free(is_prime); return; }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    /* ---- 2. Count representations ---------------------------------------- */
    uint16_t *rep_cnt = (uint16_t*)calloc((size_t)(limit + 1), sizeof(uint16_t));
    if (rep_cnt == NULL) { free(primes); free(is_prime); return; }

    for (size_t ip = 0; ip < prime_cnt; ++ip) {
        uint64_t p = primes[ip];
        for (size_t iq = ip + 1; iq < prime_cnt; ++iq) {
            uint64_t q = primes[iq];
            uint64_t pq = p * q;
            if (pq > limit) break;                    /* further q only larger */

            /* Maximal r such that sum <= limit */
            uint64_t tmp = limit - pq;
            uint64_t max_r = tmp / (p + q);
            if (max_r <= q) continue;                 /* need r > q */

            /* Find first r index */
            size_t ir = iq + 1;
            while (ir < prime_cnt && primes[ir] <= max_r) {
                uint64_t r = primes[ir];
                uint64_t sum = pq + q * r + r * p;    /* fits in 64‑bit */

                if (sum <= limit && is_prime[sum]) {
                    if (rep_cnt[sum] < 65535)
                        ++rep_cnt[sum];
                }
                ++ir;
            }
        }
    }

    /* ---- 3. Build new cache --------------------------------------------- */
    size_t new_len = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (rep_cnt[i] > 0) ++new_len;

    seq_t *new_cache = NULL;
    if (new_len > 0) {
        new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (new_cache != NULL) {
            size_t pos = 0;
            for (uint64_t i = 2; i <= limit; ++i)
                if (rep_cnt[i] > 0)
                    new_cache[pos++] = (seq_t)rep_cnt[i];
        }
    }

    /* ---- 4. Replace old cache -------------------------------------------- */
    free(term_cache);
    term_cache = new_cache;
    cache_len   = new_len;

    /* ---- 5. Free temporary arrays ---------------------------------------- */
    free(primes);
    free(rep_cnt);
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Ensure that term_cache contains at least n terms.  If not, the
 * cache is rebuilt with an increased search_limit. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;           /* already sufficient */

    while (cache_len < n) {
        recompute_cache(search_limit);
        if (cache_len >= n) break;
        search_limit *= 2ULL;            /* double the search bound */
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.  It returns the
 * n‑th value of the sequence (with offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    if (cache_len < n) return 0;
    return term_cache[(size_t)(n - 1)];     /* offset 1: a(1) is first term */
}
