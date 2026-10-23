/*
 * A120535  Semiprimes that are not the sum of 3 pentagonal numbers.
 * https://oeis.org/A120535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the n-th semiprime that cannot be expressed as the sum
 *            of three pentagonal numbers
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t current_limit = 0;                 /* current maximum number computed */
static uint32_t *spf = NULL;                   /* smallest prime factor array */
static bool *sum2 = NULL;                      /* sum of two pentagonal numbers flag */
static uint32_t *pents = NULL;                 /* pentagonal numbers list */
static size_t pents_count = 0, pents_cap = 0;   /* capacity of pents array */
static seq_t *memo = NULL;                     /* sequence cache */
static size_t memo_len = 0, memo_cap = 0;       /* cache size and capacity */

static void rebuild_limit(size_t limit)
{
    /* free previous data */
    free(spf);
    free(sum2);
    free(pents);
    spf = NULL;
    sum2 = NULL;
    pents = NULL;
    pents_count = 0;
    pents_cap = 0;

    /* build smallest prime factor array */
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) return;                       /* allocation failure */
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /* build pentagonal numbers */
    for (uint64_t k = 0;; ++k) {
        uint64_t val = k * (3 * k - 1) / 2;
        if (val > limit)
            break;
        if (pents_count == pents_cap) {
            pents_cap = pents_cap ? pents_cap * 2 : 64;
            pents = (uint32_t *)realloc(pents, pents_cap * sizeof(uint32_t));
            if (!pents) return;
        }
        pents[pents_count++] = (uint32_t)val;
    }

    /* build sum2 array */
    sum2 = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sum2) return;
    for (size_t i = 0; i < pents_count; ++i) {
        uint32_t a = pents[i];
        for (size_t j = i; j < pents_count; ++j) {
            uint64_t s = (uint64_t)a + pents[j];
            if (s > limit)
                break;
            sum2[(size_t)s] = true;
        }
    }

    /* scan new numbers for sequence terms */
    size_t start = current_limit == 0 ? 4 : current_limit + 1;
    for (uint32_t x = (uint32_t)start; x <= limit; ++x) {
        uint32_t p = spf[x];
        if (p == 0 || p == x)          /* not composite */
            continue;
        uint32_t q = x / p;
        if (q < 2 || spf[q] != q)      /* not a prime factor */
            continue;

        /* check if x is sum of three pentagonal numbers */
        bool sum3 = false;
        for (size_t k = 0; k < pents_count && pents[k] <= x; ++k) {
            uint32_t c = pents[k];
            uint32_t d = x - c;
            if (sum2[d]) {                /* found representation */
                sum3 = true;
                break;
            }
        }

        if (!sum3) {                      /* add to cache */
            if (memo_len == memo_cap) {
                memo_cap = memo_cap ? memo_cap * 2 : 256;
                memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
                if (!memo) return;
            }
            memo[memo_len++] = (seq_t)x;
        }
    }
    current_limit = limit;
}

static void ensure_terms(idx_t n)
{
    if (!initialized) {
        memo_cap = 256;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo)
            return;
        /* start with an initial reasonable limit */
        size_t init_limit = 200000;
        rebuild_limit(init_limit);
        initialized = true;
    }
    /* extend until we have at least n terms */
    while (memo_len < n) {
        size_t new_limit = current_limit ? current_limit * 2 : 200000;
        rebuild_limit(new_limit);
    }
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    if (n == 0 || n > memo_len)
        return (seq_t)0;          /* undefined for n <= 0 or beyond known */
    return memo[n - 1];
}
