/*
 * A227838  2^a(n) is the highest power of 2 dividing A005132(n).
 * https://oeis.org/A227838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2(recaman(n)) where recaman(0)=0 and recaman(k) =
 *            recaman(k-1)-k if that is positive and not already used,
 *            otherwise recaman(k) = recaman(k-1)+k. a(n) = exponent of the
 *            highest power of 2 dividing recaman(n), with offset 1 (so a(1) =
 *            v2(recaman(1)) = 0)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   valuation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/valuation.h"

/* Static state */
static idx_t recam_len = 0;          /* number of terms computed (+1 for index 0) */
static idx_t recam_cap = 0;          /* capacity of recam array */
static seq_t *recam = NULL;          /* Recamán sequence values */

static idx_t val_cache_cap = 0;      /* capacity of valuation cache */
static seq_t *val_cache = NULL;      /* cached valuations a(k) */

static size_t used_bits_words = 0;   /* number of uint64_t words for used flags */
static uint64_t *used_bits = NULL;   /* bitset storing used numbers */

/* Ensure enough space for a recam index up to 'n' (inclusive) */
static void ensure_recam_capacity(idx_t n)
{
    if (recam_cap > n) return;
    idx_t new_cap = recam_cap ? recam_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    recam = (seq_t *)realloc(recam, new_cap * sizeof(seq_t));
    val_cache = (seq_t *)realloc(val_cache, new_cap * sizeof(seq_t));
    if (!recam || !val_cache) exit(1);
    recam_cap = new_cap;
}

/* Ensure the used bitset covers the value 'v' */
static void ensure_used_bits(idx_t v)
{
    idx_t words_needed = (v >> 6) + 1;
    if (words_needed <= used_bits_words) return;
    size_t new_words = used_bits_words ? used_bits_words : 64;
    while (new_words < words_needed) new_words <<= 1;
    uint64_t *new_arr = (uint64_t *)realloc(used_bits, new_words * sizeof(uint64_t));
    if (!new_arr) exit(1);
    memset(new_arr + used_bits_words, 0,
           (new_words - used_bits_words) * sizeof(uint64_t));
    used_bits = new_arr;
    used_bits_words = new_words;
}

/* Test if a value has already appeared in the Recamán sequence */
static inline bool used(seq_t val)
{
    idx_t v = (idx_t)val;
    idx_t word = v >> 6;
    if (word >= used_bits_words) return false;
    return (used_bits[word] >> (v & 63)) & 1ULL;
}

/* Mark a value as used in the Recamán sequence */
static inline void mark_used(seq_t val)
{
    idx_t v = (idx_t)val;
    ensure_used_bits(v);
    used_bits[v >> 6] |= 1ULL << (v & 63);
}

/* Main function required by the exercise */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, but return 0 */

    /* Lazy initialization of static data */
    if (recam_len == 0) {
        recam_cap = 16;
        recam = (seq_t *)malloc(recam_cap * sizeof(seq_t));
        val_cache = (seq_t *)malloc(recam_cap * sizeof(seq_t));
        if (!recam || !val_cache) exit(1);
        recam[0] = 0;
        val_cache[0] = 0;
        recam_len = 1;
        mark_used(0);
    }

    /* Grow arrays if necessary */
    ensure_recam_capacity(n);

    /* Generate missing terms */
    for (idx_t i = recam_len; i <= n; ++i) {
        seq_t prev = recam[i - 1];
        seq_t candidate;
        bool sub_ok = false;

        if ((idx_t)prev >= i) {
            candidate = prev - (seq_t)i;
            sub_ok = !used(candidate);
        }

        seq_t next_val = sub_ok ? candidate : prev + (seq_t)i;
        recam[i] = next_val;

        /* Store valuation for this term */
        val_cache[i] = valuation(next_val);

        mark_used(next_val);
    }
    recam_len = n + 1;

    return val_cache[n];
}
