/*
 * A025056  a(n) = position of 3*n^2 in sequence A025051 (numbers of form j*k
 *          + k*i + i*j, without repetitions, where 1 <= i <= j <= k).
 * https://oeis.org/A025056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of 3*n^2 in sequence A025051 (numbers of the
 *            form i*j + j*k + i*k with 1 <= i <= j <= k, no repetition)
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Global bitset and prefix sum for numbers seen so far. */
static uint64_t *bitset = NULL;          /* one bit per integer value */
static uint64_t *prefix = NULL;          /* cumulative popcount per word */
static size_t   bitset_words = 0;        /* number of 64‑bit words in bitset */
static uint64_t current_limit = 0;       /* largest value already processed */

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n. */
static inline uint64_t floor_sqrt_uint64(uint64_t n) {
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;   /* highest power of four <= 2^64 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Expand precomputation up to NEW_LIMIT (inclusive). */
static void compute_up_to(uint64_t new_limit) {
    if (new_limit <= current_limit) return;

    /* Reallocate bitset to fit new_limit. */
    size_t new_words = (new_limit >> 6) + 1;          /* 64 bits per word */
    uint64_t *new_bitset = (uint64_t *)realloc(bitset, new_words * sizeof(uint64_t));
    if (!new_bitset) abort();
    if (new_words > bitset_words) {
        memset(new_bitset + bitset_words, 0,
               (new_words - bitset_words) * sizeof(uint64_t));
    }
    bitset = new_bitset;
    bitset_words = new_words;

    /* Generate all values <= new_limit and set bits. */
    uint64_t i_max = floor_sqrt_uint64(new_limit / 3ULL);
    for (uint64_t i = 1; i <= i_max; ++i) {
        /* Maximum j such that minimal value <= new_limit. */
        uint64_t tmp = new_limit + i * i;
        uint64_t j_max = floor_sqrt_uint64(tmp) - i;
        if (j_max < i) continue;
        for (uint64_t j = i; j <= j_max; ++j) {
            uint64_t ij = i * j;
            uint64_t denom = i + j;

            /* Determine first k that produces a value > current_limit. */
            uint64_t k_start = j;
            uint64_t val_start = ij + denom * j;   /* value at k=j */
            if (val_start <= current_limit) {
                k_start = (current_limit - ij) / denom + 1;
                if (k_start < j) k_start = j;
            }

            uint64_t k_end = (new_limit - ij) / denom;
            if (k_start > k_end) continue;

            for (uint64_t k = k_start; k <= k_end; ++k) {
                uint64_t val = ij + denom * k;
                size_t w = val >> 6;
                uint64_t mask = 1ULL << (val & 63ULL);
                bitset[w] |= mask;
            }
        }
    }

    /* Build prefix array of popcounts. */
    uint64_t *new_prefix = (uint64_t *)realloc(prefix, (new_words + 1) * sizeof(uint64_t));
    if (!new_prefix) abort();
    prefix = new_prefix;
    prefix[0] = 0;
    for (size_t w = 0; w < new_words; ++w) {
        prefix[w + 1] = prefix[w] + __builtin_popcountll(bitset[w]);
    }

    current_limit = new_limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – behaviour below 1 is irrelevant */

    /* Target value: 3*n^2 (computed in 64‑bit to avoid overflow). */
    uint64_t target = 3ULL * n * n;

    compute_up_to(target);

    /* Count distinct values strictly less than target. */
    size_t w = target >> 6;
    uint64_t mask = target & 63ULL;
    uint64_t less_before = prefix[w];

    uint64_t less_in_word = 0;
    if (mask != 0) {
        less_in_word = __builtin_popcountll(bitset[w] & ((1ULL << mask) - 1));
    }

    uint64_t rank = less_before + less_in_word + 1;   /* positions are 1‑based */

    seq_t res = rank;
    return res;
}
