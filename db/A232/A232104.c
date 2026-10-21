/*
 * A232104  Primes p with same last three digits as k, where prime(k) = p.
 * https://oeis.org/A232104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime(k) where k mod 1000 = prime(k) mod 1000 (k is the
 *            index of prime(k) in the ordered list of all primes)
 * Range:     u8 none, u16 n <= 5, u32 n <= 5839, u64 n <= 5839,
 *            u128 n <= 5839, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

#define SEG_SIZE   (1ULL << 20)       /* 1 MiB sieve segment */
#define MAX_MATCHES 200000            /* enough for n up to ~10 000 in tests */

static bool  initialized      = false;
static idx_t seg_low          = 0;    /* inclusive */
static idx_t seg_high         = 0;    /* exclusive */
static idx_t prime_index      = 0;    /* 1‑based count of primes seen */
static uint64_t *base_primes  = NULL;
static size_t   base_prime_cnt = 0;
static seq_t    matches[MAX_MATCHES];
static size_t   matches_len    = 0;

/* Ensure static initialization */
static void init_static(void)
{
    if (initialized) return;
    seg_low         = 2;
    seg_high        = seg_low + SEG_SIZE;
    prime_index     = 0;
    base_primes     = NULL;
    base_prime_cnt  = 0;
    matches_len     = 0;
    initialized = true;
}

/* Process a single segment and collect matching primes */
static void process_segment(void)
{
    /* build base primes up to sqrt(seg_high) */
    idx_t sq = int_sqrt(seg_high);
    uint64_t *tmp_bp = NULL;
    size_t    tmp_bc = 0;
    build_primes_upto(sq, &tmp_bp, &tmp_bc);
    free(base_primes);
    base_primes    = tmp_bp;
    base_prime_cnt = tmp_bc;

    /* sieve current segment */
    size_t seg_len = (size_t)(seg_high - seg_low);
    char *is_comp = (char *)calloc(seg_len, 1);
    if (!is_comp) return;  /* allocation failure – abort */

    for (size_t bi = 0; bi < base_prime_cnt; ++bi) {
        uint64_t p = base_primes[bi];
        uint64_t p2 = p * p;
        if (p2 > seg_high - 1) break;
        uint64_t start = seg_low <= p2 ? p2
                      : ((seg_low + p - 1) / p) * p;
        for (uint64_t j = start; j < seg_high; j += p)
            is_comp[(size_t)(j - seg_low)] = 1;
    }

    /* walk through segment, collect primes that satisfy the condition */
    for (idx_t i = seg_low; i < seg_high; ++i) {
        if (i >= 2 && !is_comp[(size_t)(i - seg_low)]) {
            ++prime_index;
            uint64_t p = (uint64_t)i;
            if ((p % 1000ULL) == (prime_index % 1000ULL)) {
                if (matches_len < MAX_MATCHES) {
                    /* assign to the pre‑allocated array */
                    matches[matches_len++] = (seq_t)p;
                }
            }
        }
    }

    free(is_comp);
}

/* Ensure we have at least n matches ready. */
static void ensure_matches(idx_t n)
{
    init_static();
    while (matches_len < (size_t)n) {
        process_segment();
        seg_low  = seg_high;
        seg_high += SEG_SIZE;
    }
}

/* exported function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out‑of‑bounds guard */
    ensure_matches(n);
    return matches[n - 1];
}
