/*
 * A078722  a(n) = prime(n*(n+1)/2+2).
 * https://oeis.org/A078722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime( n*(n+1)/2 + 2 ) (prime(1)=2)
 * Range:     u8 n <= 9, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   upper_bound_prime_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/upper_bound_prime_n.h"

/*--- globals -------------------------------------------------------*/
static uint64_t *primes    = NULL;   /* primes generated so far        */
static size_t   primes_cap = 0;
static size_t   primes_len = 0;      /* number of primes stored        */

static uint64_t *small_primes = NULL;
static size_t   small_cap = 0;
static size_t   small_len = 0;
static idx_t    small_limit = 0;     /* max sqrt value processed       */

static uint64_t current_limit = 2;   /* largest prime in primes list    */

/*--- helpers -------------------------------------------------------*/
static void ensure_primes_cap(size_t needed) {
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

static void recompute_small_primes(idx_t limit) {
    idx_t needed = (idx_t)(sqrt((double)limit)) + 1;
    if (needed <= small_limit) return;             /* already enough */

    if (small_cap < (size_t)needed) {
        size_t new_cap = small_cap ? small_cap * 2 : 256;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(small_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        small_primes = tmp;
        small_cap = new_cap;
    }

    bool *mark = (bool*)calloc(needed + 1, 1);
    if (!mark) abort();

    mark[0] = mark[1] = true;
    for (idx_t p = 2; p * p <= needed; ++p) {
        if (!mark[p]) {
            for (idx_t j = p * p; j <= needed; j += p)
                mark[j] = true;
        }
    }

    /* count primes first to know how much storage is needed */
    size_t cnt = 0;
    for (idx_t i = 2; i <= needed; ++i)
        if (!mark[i]) ++cnt;

    if (small_len + cnt > small_cap) {
        size_t new_cap = small_cap ? small_cap * 2 : 256;
        while (new_cap < small_len + cnt) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(small_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        small_primes = tmp;
        small_cap = new_cap;
    }

    for (idx_t i = 2; i <= needed; ++i)
        if (!mark[i]) small_primes[small_len++] = (uint64_t)i;

    free(mark);
    small_limit = needed;
}

/* Ensure we have at least 'need' primes in the array (1‑indexed) */
static void generate_primes_to_index(idx_t need) {
    if (need <= primes_len) return;

    /* Prepare small primes list */
    idx_t target_limit = upper_bound_prime_n(need);
    recompute_small_primes(target_limit);

    /* If we already have enough primes after small_primes (rare) */
    if (primes_len >= need) return;

    /* Ensure prime list contains 2 at start */
    if (primes_len == 0) {
        ensure_primes_cap(1);
        primes[0] = 2;
        primes_len = 1;
        current_limit = 2;
    }

    /* Segmented sieve */
    const idx_t seg_size = 2000000;          /* number of integers per segment */
    idx_t seg_start = current_limit + 1;
    if (seg_start < 3) seg_start = 3;

    while (seg_start <= target_limit && primes_len < need) {
        idx_t seg_end = seg_start + seg_size - 1;
        if (seg_end > target_limit) seg_end = target_limit;

        size_t seg_len = seg_end - seg_start + 1;
        char *is_comp = (char*)calloc(seg_len, 1);
        if (!is_comp) abort();

        /* Mark composites using small primes */
        for (size_t ip = 0; ip < small_len; ++ip) {
            uint64_t p = small_primes[ip];
            uint64_t p2 = p * p;
            if (p2 > seg_end) break;
            idx_t start = seg_start <= p2 ? p2 : ((seg_start + p - 1) / p) * p;
            for (idx_t j = start; j <= seg_end; j += p)
                is_comp[j - seg_start] = 1;
        }

        /* Collect primes */
        for (size_t i = 0; i < seg_len; ++i) {
            idx_t num = seg_start + i;
            if (!is_comp[i]) {
                /* num is prime */
                ensure_primes_cap(primes_len + 1);
                primes[primes_len++] = (uint64_t)num;
                if (num > current_limit) current_limit = num;
                if (primes_len >= need) break;
            }
        }

        free(is_comp);
        seg_start = seg_end + 1;
    }

    /* If we still don't have enough primes, double the limit and try again */
    if (primes_len < need) {
        target_limit *= 2;
        generate_primes_to_index(need);
    }
}

/*--- public function -----------------------------------------------*/
seq_t a(idx_t n)
{
    /* compute the triangular number n*(n+1)/2 using seq_t */
    idx_t tri = (idx_t)((seq_t)n * ((seq_t)n + 1) / 2);
    idx_t k = tri + 2;   /* 1‑based index of the required prime */

    generate_primes_to_index(k);
    return (seq_t)primes[k - 1];   /* primes[k-1] is the k‑th prime (1‑based) */
}
