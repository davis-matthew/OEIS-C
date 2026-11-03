/*
 * A063966  Number of Abelian groups of order <= n.
 * https://oeis.org/A063966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≤n} Π_{p^e||k} p(e) where p(e) is the partition
 *            number of e
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9239 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

static seq_t *cache = NULL;     /* cumulative counts a(0)…a(current) */
static idx_t cache_size = 0;    /* largest index cached (>=0) */

static idx_t *spf = NULL;       /* smallest prime factor table up to spf_size */
static idx_t spf_size = 0;      /* current size of spf table */

/* ------------------------------------------------------------------ */
/*  Partition numbers up to PART_MAX                                */
#define PART_MAX 64
static seq_t part[PART_MAX + 1] = {0};
static bool part_initialized = false;

static void init_partition_table(void)
{
    if (part_initialized)
        return;
    part[0] = 1;
    for (idx_t i = 1; i <= PART_MAX; ++i) {
        for (idx_t j = i; j <= PART_MAX; ++j) {
            part[j] += part[j - i];
        }
    }
    part_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the SPF table contains all integers <= limit       */
static void ensure_spf(idx_t limit)
{
    if (spf_size >= limit)
        return;
    if (spf != NULL) {
        free(spf);
        spf = NULL;
    }
    build_spf(limit, &spf);
    spf_size = limit;
}

/* ------------------------------------------------------------------ */
/*  Ensure the cache contains a(i) for all i ≤ idx                  */
static void ensure_cache(idx_t idx)
{
    if (cache_size >= idx)
        return;                      /* already available                     */

    /* ------------------------------------------------------------------ */
    /*  Allocate or grow the cache array                                */
    if (cache == NULL) {
        cache = alloc_cache(idx + 1); /* includes a(0) */
        cache[0] = 0;
        cache_size = 0;
    } else if (cache_size < idx) {
        seq_t *tmp = alloc_cache(idx + 1);
        for (idx_t i = 0; i <= cache_size; ++i)
            tmp[i] = cache[i];
        mem_free(cache);
        cache = tmp;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute missing values                                           */
    for (idx_t i = cache_size + 1; i <= idx; ++i) {
        seq_t groups = 1;
        idx_t m = i;
        while (m > 1) {
            idx_t p = spf[m];
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            groups *= part[exp];
        }
        cache[i] = cache[i - 1] + groups;
    }
    cache_size = idx;
}

/* ------------------------------------------------------------------ */
/*  Sequence function: returns a(n) for n ≥ 1                        */
seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for calls with n=0            */
        return 0;

    init_partition_table();
    ensure_spf(n);
    ensure_cache(n);
    return cache[n];
}
