/*
 * A156998  The first of two primes in two-prime centuries.
 * https://oeis.org/A156998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition: the first prime in every 100‑consecutive‑integer
   block (century) that contains exactly two primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/build_primes_upto.h"

/* ---------- static data -------------------------------- */

#define SEGMENT_SIZE   (1u << 20)   /* 1 048 576 numbers per segment */
#define INITIAL_PRIME_LIMIT 1000000 /* primes up to 1 000 000 are plenty */

static seq_t   *cache      = NULL;
static size_t   cache_cap  = 0;
static size_t   cache_size = 0;

static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;

static unsigned char *segment = NULL;   /* marks composites in current segment */
static uint64_t current_start = 2;      /* next number to process */

static size_t state_count       = 0;    /* primes seen in current century */
static seq_t  state_first_prime = 0;    /* first prime of current century */

/* ---------- helpers ------------------------------------ */

static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        if (cache_cap == 0)
            cache_cap = 16;
        else
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

static void ensure_prime_table(void)
{
    if (prime_list)
        return;
    size_t cnt;
    build_primes_upto(INITIAL_PRIME_LIMIT, &prime_list, &cnt);
    prime_count = cnt;
}

static void ensure_cache_size(idx_t n)
{
    ensure_prime_table();

    if (!segment)
        segment = (unsigned char *)malloc(SEGMENT_SIZE);

    /* Scan segments until we have at least n entries in the cache */
    while (cache_size < (size_t)n) {
        uint64_t seg_start = current_start;
        uint64_t seg_end   = seg_start + SEGMENT_SIZE - 1;
        size_t   seg_len   = SEGMENT_SIZE;

        /* Mark composites in [seg_start, seg_end]. */
        memset(segment, 0, seg_len);

        for (size_t pi = 0; pi < prime_count && prime_list[pi] * prime_list[pi] <= seg_end; ++pi) {
            uint64_t p = prime_list[pi];
            uint64_t first = (seg_start + p - 1) / p * p;
            if (first < p * 2)
                first = p * 2;
            for (uint64_t j = first; j <= seg_end; j += p) {
                segment[(size_t)(j - seg_start)] = 1;
            }
        }

        /* Scan numbers and detect centuries with exactly two primes. */
        for (size_t idx = 0; idx < seg_len; ++idx) {
            uint64_t num = seg_start + idx;
            bool is_prime = (num >= 2 && !segment[idx]);

            if (is_prime) {
                if (state_count == 0)
                    state_first_prime = (seq_t)num;
                ++state_count;
            }

            if ((num % 100u) == 99u) { /* end of a century */
                if (state_count == 2)
                    cache_append(state_first_prime);
                state_count = 0;
            }
        }

        current_start = seg_end + 1;
    }
}

/* ---------- public function -------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* prime list, segment, and cache will be lazily initialized. */
        initialized = true;
    }
    ensure_cache_size(n);
    return cache[n - 1];
}
