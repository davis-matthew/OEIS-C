/*
 * A075562  Rearrangement of natural numbers such that every partial sum is a
 *          proper prime power.
 * https://oeis.org/A075562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   build_primes_upto, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075562: rearrangement of natural numbers such that every
   partial sum is a proper prime power.  The implementation
   builds the sequence by greedily choosing the smallest unused
   difference that results in the next prime‑power partial sum.
   The sequence is computed lazily and memoised. */

#ifdef __cplusplus
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdbool>
#define ALLOC_SEQ(sz) mem_new(seq_t, (sz))
#define REALLOC_SEQ(ptr,sz) mem_resize(seq_t, (ptr), (sz))
#else
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ALLOC_SEQ(sz) mem_new(seq_t, (sz))
#define REALLOC_SEQ(ptr,sz) mem_resize(seq_t, (ptr), (sz))
#endif

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* ---------- static data ------------------------------------------------- */
static seq_t *primePowers   = NULL;   /* all prime powers >=4, sorted           */
static size_t pp_cnt        = 0;       /* number of prime powers stored          */
static seq_t pp_limit       = 0;       /* largest prime power currently stored   */
static uint64_t *primeList  = NULL;   /* primes up to sqrt( pp_limit )         */
static size_t primeCnt      = 0;       /* number of primes                       */
static idx_t   primeLimit   = 0;       /* upper bound for primeList              */
static idx_t   cur_pp_idx   = 0;       /* index in primePowers > current sum    */

static seq_t *cache         = NULL;   /* a(1)…a(current)                        */
static size_t cache_cap     = 0;
static size_t cache_len     = 0;
static seq_t  sum           = 0;       /* current cumulative sum                */

static uint8_t *usedDiff    = NULL;   /* 1 iff a difference has been used      */
static size_t  usedCap      = 0;

static bool initialized     = false;

/* ---------- helper: extend the prime power table ----------------------- */
static void extendPrimePowers(idx_t new_limit)
{
    /* build prime list up to sqrt(new_limit) if needed */
    idx_t sqrt_lim = isqrt(new_limit) + 1;
    if (primeLimit < sqrt_lim) {
        uint64_t *tmp;
        size_t tmp_cnt;
        build_primes_upto(sqrt_lim, &tmp, &tmp_cnt);
        if (primeList) free(primeList);
        primeList = tmp;
        primeCnt  = tmp_cnt;
        primeLimit = sqrt_lim;
    }

    /* create temporary array for new prime powers */
    size_t cap = 1024;
    seq_t *temps = ALLOC_SEQ(cap);
    size_t cnt = 0;

    for (size_t i = 0; i < primeCnt; ++i) {
        seq_t p = (seq_t)primeList[i];
        seq_t val = p * p;      /* start with exponent 2 */
        while (val <= (seq_t)new_limit) {
            if (cnt == cap) {
                cap <<= 1;
                temps = REALLOC_SEQ(temps, cap);
            }
            temps[cnt++] = val;
            if (val > (seq_t)new_limit / p) break;
            val *= p;
        }
    }

    mem_qsort(temps, cnt, sizeof(seq_t), cmp_seq_t);

    if (primePowers) mem_free(primePowers);
    primePowers = temps;
    pp_cnt = cnt;
    pp_limit = (seq_t)new_limit;
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* first term a(1) = 1 */
        cache_cap = 16;
        cache = ALLOC_SEQ(cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
        sum = (seq_t)1;

        usedCap   = 16;
        usedDiff  = (uint8_t*)calloc(usedCap, sizeof(uint8_t));
        usedDiff[1] = 1;     /* difference 1 already used */

        extendPrimePowers(2000);   /* reasonable start */
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        /* advance cur_pp_idx to first prime power strictly larger than sum */
        while (cur_pp_idx < pp_cnt && primePowers[cur_pp_idx] <= sum)
            ++cur_pp_idx;

        idx_t idx = cur_pp_idx;
        while (true) {
            if (idx >= pp_cnt) {   /* need larger prime powers */
                seq_t tmp_limit = pp_limit * 2 + 1000;
                idx_t new_limit = (tmp_limit > UINT64_MAX) ? UINT64_MAX
                                                             : (idx_t)tmp_limit;
                extendPrimePowers(new_limit);
                idx = 0;
                while (idx < pp_cnt && primePowers[idx] <= sum)
                    ++idx;
                cur_pp_idx = idx;
                continue;
            }

            seq_t candidate = primePowers[idx];
            seq_t d = candidate - sum;   /* candidate difference */

            if ((seq_t)d >= (seq_t)usedCap) {   /* grow usedDiff array */
                size_t new_cap = usedCap ? usedCap : 64;
                while ((seq_t)new_cap <= d) new_cap <<= 1;
                usedDiff = (uint8_t*)realloc(usedDiff, new_cap * sizeof(uint8_t));
                memset(usedDiff + usedCap, 0,
                       (new_cap - usedCap) * sizeof(uint8_t));
                usedCap = new_cap;
            }

            if (!usedDiff[(size_t)d]) {          /* first unused difference */
                if (cache_len >= cache_cap) {    /* grow cache */
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = REALLOC_SEQ(cache, cache_cap);
                }
                cache[cache_len++] = d;
                sum = candidate;
                usedDiff[(size_t)d] = 1;
                cur_pp_idx = idx;
                break;
            }
            ++idx;
        }
    }

    return cache[(size_t)n - 1];
}
