/*
 * A078898  Number of times the smallest prime factor of n is the smallest
 *          prime factor for numbers <= n; a(0)=0, a(1)=1.
 * https://oeis.org/A078898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _ensure_cap, _spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A078898: a(n) = number of m ≤ n such that
 * smallest_prime_factor(m) = smallest_prime_factor(n).
 * a(0)=0, a(1)=1.
 * The implementation builds the values progressively and memoises
 * them so that repeated calls are O(1) after initialisation.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_ensure_cap.h"
#include "../utils/_spf.h"

/* Memoised storage for a(n) */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;        /* allocated size of a_cache */
static seq_t *prim_cnt = NULL; /* count of numbers with each prime lpf */
static size_t pcap = 0;        /* allocated size of prim_cnt */
static idx_t computed_max = 0; /* largest n processed so far */

seq_t a(idx_t n)
{
    /* Initialise caches if first call */
    if (!a_cache) {
        _ensure_cap(n + 1, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[0] = 0;              /* a(0)=0 */
        computed_max = 0;
    }
    if (!prim_cnt) {
        _ensure_cap(n + 1, &pcap, (void **)&prim_cnt, sizeof(seq_t));
    }

    /* Extend caches to hold up to index n */
    if (n + 1 > a_cap) _ensure_cap(n + 1, &a_cap, (void **)&a_cache, sizeof(seq_t));
    if (n + 1 > pcap) _ensure_cap(n + 1, &pcap, (void **)&prim_cnt, sizeof(seq_t));

    /* Compute values up to n if needed */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        idx_t p = _spf(i);                 /* smallest prime factor of i */
        prim_cnt[p] = (seq_t)(prim_cnt[p] + (seq_t)1);
        a_cache[i] = prim_cnt[p];
    }
    computed_max = n;

    return a_cache[n];
}
