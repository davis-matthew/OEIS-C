/*
 * A098423  Primes occurring in exactly three prime triples (p,q,r) with
 *          p<q<r=p+6.
 * https://oeis.org/A098423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A098423: primes occurring in exactly three prime triples
 * (p,q,r) with p<q<r=p+6. Computed by sieving primes up to a limit
 * and counting how often each prime appears in such triples. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: binary search in a sorted array of uint64_t.
   Returns index of val if found, otherwise prime_cnt. */
static size_t find_prime_index(const uint64_t *primes, size_t prime_cnt,
                               uint64_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == val) return mid;
        if (primes[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return prime_cnt;
}

/* Build all terms of the sequence up to the declared limit.
   Output: sequence length in *out_len and pointer to array in *out_seq.
   The caller must free *out_seq. */
static void build_cache_up_to(idx_t limit, size_t *out_len, seq_t **out_seq)
{
    /* ----- sieve primes up to limit ----- */
    bool *is_prime = (bool*)malloc((size_t)limit + 1);
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (!is_prime[i]) continue;
        for (size_t j = i * i; j <= (size_t)limit; j += i)
            is_prime[j] = false;
    }

    /* collect primes into an array, reallocating if necessary */
    size_t primes_cap = limit / 10 + 100;
    uint64_t *primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes,
                                                   primes_cap * sizeof(uint64_t));
                primes = tmp;
            }
            primes[prime_cnt++] = (uint64_t)i;
        }
    }
    free(is_prime);

    /* ----- count occurrences in triples ----- */
    uint16_t *cnt = (uint16_t*)calloc(prime_cnt, sizeof(uint16_t));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t r = p + 6;
        if (r > limit) continue;               // outer prime beyond limit
        size_t j = find_prime_index(primes, prime_cnt, r);
        if (j == prime_cnt) continue;          // outer prime not found
        ++cnt[i];                             // count outer left
        ++cnt[j];                             // count outer right
        for (size_t k = i + 1; k < j; ++k)    // inner primes
            ++cnt[k];
    }

    /* ----- collect primes with exactly three appearances ----- */
    seq_t *seq = mem_new(seq_t, prime_cnt);
    size_t seq_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (cnt[i] == 3) {
            seq[seq_cnt++] = (seq_t)primes[i];
        }
    }

    free(primes);
    free(cnt);

    *out_len = seq_cnt;
    *out_seq = seq;
}

/* Cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static idx_t  current_limit = 0;

/* External function required by the evaluator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1 */
    if (n <= seq_len)                          /* already cached */
        return seq_cache[n - 1];

    /* need more terms: rebuild with larger limits */
    idx_t limit = 100000;                      /* starting sieve size */
    seq_t *new_cache = NULL;
    size_t new_len = 0;
    do {
        /* free previous attempt if any */
        if (new_cache) mem_free(new_cache);

        build_cache_up_to(limit, &new_len, &new_cache);
        limit <<= 1;              /* double for next trial if necessary */
    } while (new_len < (size_t)n);

    /* replace old cache */
    if (seq_cache) mem_free(seq_cache);
    seq_cache = new_cache;
    seq_len  = new_len;
    current_limit = limit >> 1;   /* remember last used limit */

    return seq_cache[n - 1];
}
