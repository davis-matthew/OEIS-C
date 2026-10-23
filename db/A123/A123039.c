/*
 * A123039  Prime sums of 11 positive 5th powers.
 * https://oeis.org/A123039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1211, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitset_words, mask_last_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes that are sums of 11 positive fifth powers (OEIS A123039) */
/* Computed by a 2‑step dynamic programming + primality sieve */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/bitset_words.h"
#include "../utils/mask_last_word.h"

/* ------------------------------------------------------------------ */
/* Bitset helpers                                                    */

static inline bool bit_test(const uint64_t *arr, idx_t pos) {
    size_t word = (size_t)(pos >> 6);
    size_t bit  = (size_t)(pos & 63);
    return (arr[word] >> bit) & 1ULL;
}

/* Shift-and-OR helper: add one term of size 'shift' to the DP bitsets.
   If 'swap' is true the parity is toggled (odd power), otherwise it stays. */
static void add_shift(const uint64_t *src_even, const uint64_t *src_odd,
                      uint64_t *dst_even, uint64_t *dst_odd,
                      size_t words, idx_t shift, bool swap) {
    size_t word_shift = (size_t)(shift >> 6);
    size_t bit_shift  = (size_t)(shift & 63);
    for (size_t w = 0; w < words; ++w) {
        long long src_index = (long long)w - (long long)word_shift;
        uint64_t part_even = 0, part_odd = 0;
        if (src_index >= 0) {
            size_t s = (size_t)src_index;
            part_even = src_even[s] << bit_shift;
            part_odd  = src_odd [s] << bit_shift;
            if (bit_shift && s > 0) {
                part_even |= src_even[s - 1] >> (64 - bit_shift);
                part_odd  |= src_odd [s - 1] >> (64 - bit_shift);
            }
        }
        if (!swap) {
            dst_even[w] |= part_even;
            dst_odd [w] |= part_odd;
        } else {
            dst_odd [w] |= part_even;
            dst_even[w] |= part_odd;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Sequence generator                                                */
/* ------------------------------------------------------------------ */
static seq_t *seq_terms   = NULL;   /* cached list of primes */
static idx_t  seq_count   = 0;      /* number cached */
static bool   seq_ready   = false;  /* initialization flag */

/* Helper to compute all terms up to a given upper bound */
static void compute_terms(idx_t upper) {
    /* 1. Precompute 5th powers <= upper */
    idx_t max_base = 0;
    while (true) {
        idx_t p = 1;
        for (int i = 0; i < 5; ++i) p *= (idx_t)(max_base + 1);
        if (p > upper) break;
        ++max_base;
    }
    uint64_t *pows = (uint64_t *)malloc((size_t)max_base * sizeof(uint64_t));
    for (idx_t i = 1; i <= max_base; ++i) {
        uint64_t v = 1;
        for (int j = 0; j < 5; ++j) v *= (uint64_t)i;
        pows[i - 1] = v;
    }

    /* 2. DP with bitsets */
    idx_t bits_needed = upper + 1;
    size_t words = bitset_words(bits_needed);
    uint64_t *cur_even = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *cur_odd  = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *next_even = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *next_odd  = (uint64_t *)calloc(words, sizeof(uint64_t));

    cur_even[0] = 1ULL;   /* sum 0 with even parity (0 odd terms) */

    for (idx_t term = 1; term <= 11; ++term) {
        memset(next_even, 0, words * sizeof(uint64_t));
        memset(next_odd,  0, words * sizeof(uint64_t));
        for (idx_t pi = 0; pi < max_base; ++pi) {
            bool swap = (pows[pi] & 1ULL) ? true : false;
            add_shift(cur_even, cur_odd,
                      next_even, next_odd,
                      words, (idx_t)pows[pi], swap);
        }
        mask_last_word(next_even, words, bits_needed);
        mask_last_word(next_odd,  words, bits_needed);
        /* swap current and next */
        uint64_t *tmp = cur_even; cur_even = next_even; next_even = tmp;
        tmp = cur_odd;  cur_odd  = next_odd;  next_odd  = tmp;
    }

    /* 3. Sieve primes up to upper */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(upper + 1));
    memset(is_prime, 1, (size_t)(upper + 1));
    if (upper >= 0) is_prime[0] = 0;
    if (upper >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= upper; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= upper; j += i)
                is_prime[j] = 0;

    /* 4. Collect primes that are representable */
    size_t cap = 20000;
    seq_terms = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_count = 0;
    for (idx_t p = 2; p <= upper; ++p) {
        if (is_prime[p] && bit_test(cur_odd, p)) {
            if (seq_count == cap) {
                cap *= 2;
                seq_terms = (seq_t *)realloc(seq_terms, cap * sizeof(seq_t));
            }
            seq_terms[seq_count++] = (seq_t)p;
        }
    }

    /* 5. Clean up */
    free(pows);
    free(cur_even);
    free(cur_odd);
    free(next_even);
    free(next_odd);
    free(is_prime);

    seq_ready = true;
}

/* ------------------------------------------------------------------ */
/* Public API                                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1 */
    if (!seq_ready) {
        /* Upper bound chosen to safely contain more than 10 000 terms */
        compute_terms(10000000);
    }
    /* If still not enough (unlikely), recompute with larger bound */
    while (n > seq_count) {
        compute_terms(seq_count * 5);   /* increase bound adaptively */
    }
    return seq_terms[n - 1];
}
