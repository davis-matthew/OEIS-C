/*
 * A144559  a(n) = number of triples [i,j,k] with i+j+k = n, i an odd prime, j
 *          an odd Fibonacci number and k a positive Fibonacci number.
 * https://oeis.org/A144559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples [i,j,k] with i+j+k = n, where i is an
 *            odd prime, j is an odd Fibonacci number, and k is a positive
 *            Fibonacci number
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* cached data */
static uint64_t *primes = NULL;          /* odd primes > 2 */
static size_t primes_cnt = 0;
static idx_t *fibs = NULL;               /* all Fibonacci numbers ≥ 1 */
static size_t fibs_cnt = 0;
static idx_t *odd_fibs = NULL;           /* odd Fibonacci numbers (unique) */
static size_t odd_fibs_cnt = 0;
static idx_t cache_limit = 0;            /* maximum n for which data is ready */

/* memoisation */
static seq_t *memo = NULL;
static bool *computed = NULL;
static size_t memo_len = 0;

/* binary search for a Fibonacci number */
static bool fib_member(idx_t val)
{
    size_t lo = 0, hi = fibs_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        idx_t m = fibs[mid];
        if (m == val) return true;
        if (m < val) lo = mid + 1;
        else          hi = mid;
    }
    return false;
}

/* extend cached data up to at least `limit` */
static void extend_cache(idx_t limit)
{
    if (limit <= cache_limit) return;

    /* ---- Fibonacci numbers ---- */
    if (fibs == NULL) {
        fibs_cnt = 2;
        fibs = (idx_t*)malloc(fibs_cnt * sizeof(idx_t));
        fibs[0] = 1;
        fibs[1] = 1;
    }
    idx_t a = fibs[fibs_cnt-2];
    idx_t b = fibs[fibs_cnt-1];
    while (true) {
        idx_t c = a + b;
        if (c > limit) break;
        idx_t *tmp = (idx_t*)realloc(fibs, (fibs_cnt+1)*sizeof(idx_t));
        if (tmp == NULL) abort();
        fibs = tmp;
        fibs[fibs_cnt++] = c;
        a = b;
        b = c;
    }

    /* ---- Odd Fibonacci numbers (unique) ---- */
    free(odd_fibs);
    odd_fibs_cnt = 0;
    odd_fibs = (idx_t*)malloc(fibs_cnt * sizeof(idx_t));
    for (size_t i = 0; i < fibs_cnt; ++i) {
        if ((fibs[i] & 1ULL) != 0) {
            if (odd_fibs_cnt == 0 || fibs[i] != odd_fibs[odd_fibs_cnt-1]) {
                odd_fibs[odd_fibs_cnt++] = fibs[i];
            }
        }
    }

    /* ---- Primes ---- */
    uint64_t *tmp_primes = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(limit, &tmp_primes, &tmp_cnt);
    free(primes);
    primes_cnt = 0;
    primes = (uint64_t*)malloc(tmp_cnt * sizeof(uint64_t));
    for (size_t i = 0; i < tmp_cnt; ++i) {
        uint64_t p = tmp_primes[i];
        if (p > 2 && (p & 1ULL) != 0) {
            primes[primes_cnt++] = p;
        }
    }
    free(tmp_primes);

    cache_limit = limit;

    /* ---- Memoisation arrays ---- */
    if (cache_limit + 1 > memo_len) {
        size_t new_len = (size_t)cache_limit + 1;
        seq_t *mtmp = (seq_t*)realloc(memo, new_len * sizeof(seq_t));
        if (mtmp == NULL) abort();
        memo = mtmp;

        bool *ctmp = (bool*)realloc(computed, new_len * sizeof(bool));
        if (ctmp == NULL) abort();
        computed = ctmp;

        for (size_t i = memo_len; i < new_len; ++i) {
            memo[i] = 0;
            computed[i] = false;
        }
        memo_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n < memo_len && computed[n]) return memo[n];

    extend_cache(n);

    seq_t count = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        if (p > n - 2) break;            /* at least 1 for j and k */
        idx_t rem = n - p;               /* j + k */
        for (size_t fi = 0; fi < odd_fibs_cnt; ++fi) {
            idx_t j = odd_fibs[fi];
            if (j > rem - 1) break;      /* k must be >= 1 */
            idx_t k = rem - j;
            if (k < 1) continue;
            if (fib_member(k)) {
                count += 1;
            }
        }
    }

    if (n < memo_len) {
        memo[n] = count;
        computed[n] = true;
    }
    return count;
}
