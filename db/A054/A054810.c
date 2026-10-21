/*
 * A054810  Third term of strong prime 5-tuples: p(m-1)-p(m-2) > p(m)-p(m-1) >
 *          p(m+1)-p(m) > p(m+2)-p(m+1).
 * https://oeis.org/A054810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the third prime p(m) of a strong prime 5‑tuple defined
 *            by p(m-1)-p(m-2) > p(m)-p(m-1) > p(m+1)-p(m) > p(m+2)-p(m+1)
 *            (OEIS A054810)
 * Range:     u8 none, u16 n <= 171, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SEG_SIZE (1 << 15)   /* 32768   segment size for the sieve   */

static seq_t *cache = NULL;          /* cached sequence values          */
static size_t cache_len = 0;         /* number of elements already cached*/
static size_t cache_cap = 0;         /* allocated capacity               */

static uint32_t *small_primes = NULL;/* primes up to 1 000 000          */
static size_t small_prime_cnt = 0;    /* number of primes in small_primes*/

static uint64_t seg_start = 2;       /* start of the next segment to sieve*/
static uint64_t last5[5];            /* last five primes seen            */
static int last_cnt = 0;             /* how many of last5 are valid      */

/* ------------------------------------------------------------------ */
/* Prepare the list of small primes up to 1 000 000 (used for all      */
/* subsequent segmented sieving).  Uses a simple sieve.                */
/* ------------------------------------------------------------------ */
static void prepare_small_primes(void)
{
    if (small_primes != NULL) return;        /* already done */

    const uint32_t LIMIT = 1000000;
    bool *mark = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    memset(mark, 0, (LIMIT + 1) * sizeof(bool));

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (!mark[i])
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                mark[(size_t)j] = true;

    small_prime_cnt = 0;
    small_primes = (uint32_t *)malloc(80000 * sizeof(uint32_t)); /* enough for 1e6 */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!mark[i])
            small_primes[small_prime_cnt++] = i;

    free(mark);
}

/* ------------------------------------------------------------------ */
/* Ensure that cache contains at least n elements by continuing to    */
/* generate primes and test the strong‐prime 5‑tuple condition.        */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* already ready      */

    prepare_small_primes();                       /* one-time work      */

    static bool seg[SEG_SIZE];                    /* boolean segment    */
    while (cache_len < (size_t)n) {
        uint64_t L = seg_start;
        uint64_t R = L + SEG_SIZE - 1;
        memset(seg, 0, SEG_SIZE * sizeof(bool));

        /* mark composites inside [L,R] */
        for (size_t i = 0; i < small_prime_cnt; ++i) {
            uint32_t p = small_primes[i];
            uint64_t p2 = (uint64_t)p * p;
            if (p2 > R) break;
            uint64_t start = (L + p - 1) / p * p;
            if (start < p2) start = p2;
            for (uint64_t j = start; j <= R; j += p)
                seg[(size_t)(j - L)] = true;
        }

        /* collect primes in this segment */
        for (size_t idx = 0; idx < SEG_SIZE; ++idx) {
            uint64_t num = L + idx;
            if (num < 2) continue;
            if (!seg[idx]) {
                /* new prime found: update the sliding window of last five */
                for (int k = 4; k > 0; --k) last5[k] = last5[k - 1];
                last5[0] = (uint64_t)num;
                if (last_cnt < 5) ++last_cnt;

                if (last_cnt == 5) {
                    uint64_t g1 = last5[1] - last5[0];
                    uint64_t g2 = last5[2] - last5[1];
                    uint64_t g3 = last5[3] - last5[2];
                    uint64_t g4 = last5[4] - last5[3];
                    if (g1 > g2 && g2 > g3 && g3 > g4) {
                        if (cache_len == cache_cap) {
                            cache_cap = cache_cap ? cache_cap * 2 : 64;
                            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                            if (!cache) { free(small_primes); abort(); }
                        }
                        cache[cache_len++] = (seq_t)last5[2]; /* third prime of the tuple */
                    }
                }
            }
        }

        seg_start = R + 1;   /* advance to next segment */
    }
}

/* ------------------------------------------------------------------ */
/* The required sequence accessor.  Called many times, possibly out   */
/* of order; the static cache guarantees efficient future calls.     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset 1, but safe */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
