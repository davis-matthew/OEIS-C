/*
 * A132665  a(1)=1, a(2)=3, a(n) = a(n-1) + n if the minimal positive integer
 *          not yet in the sequence is greater than a(n-1), else a(n) =
 *          a(n-1)-1.
 * https://oeis.org/A132665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(n) = a(n-1)+n if the minimal positive integer
 *            not yet in the sequence is ≤ a(n-1); otherwise a(n)=a(n-1)-1.
 *            This implementation follows the recurrence directly
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Persistent state for memoisation */
static idx_t max_n = 0;               /* largest index computed so far     */
static seq_t *values = NULL;          /* 1‑based array of computed terms   */
static size_t values_cap = 0;         /* allocated capacity for 'values'   */

/* Bitset that stores which positive integers have already appeared
   in the sequence.  `used_bits_cap` is the number of bits currently
   allocated (not the number of words). */
static uint64_t *used_bits = NULL;    /* array of 64‑bit words */
static idx_t used_bits_cap = 0;       /* number of bits allocated */

/* Expand the `values` array to hold at least `n` indices (1‑based). */
static void ensure_values_capacity(idx_t n)
{
    if (n <= (idx_t)values_cap) return;
    size_t new_cap = values_cap ? values_cap * 2 : 8;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(values, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) abort();          /* fatal if allocation fails */
    values = new_arr;
    values_cap = new_cap;
}

/* Expand the bitset to be able to record numbers up to `val`.  The
   argument `val` is a positive integer that has appeared in the
   sequence. */
static void ensure_used_capacity(idx_t val)
{
    if ((size_t)val < used_bits_cap) return;
    /* Determine a new capacity that is a power of two and big enough. */
    size_t new_cap = used_bits_cap ? used_bits_cap * 2 : 128; /* bits */
    while (new_cap <= (size_t)val) new_cap <<= 1;

    /* Compute how many 64‑bit words are needed for the new size. */
    size_t new_words = (new_cap + 63) >> 6;
    size_t old_words = (used_bits_cap + 63) >> 6;

    uint64_t *new_bits = (uint64_t *)realloc(used_bits,
                                             new_words * sizeof(uint64_t));
    if (!new_bits) abort();
    /* Zero the newly added words. */
    if (new_words > old_words)
        memset(new_bits + old_words, 0,
               (new_words - old_words) * sizeof(uint64_t));
    used_bits = new_bits;
    used_bits_cap = new_cap;
}

/* Mark `val` as used in the sequence. */
static void used_set(seq_t val)
{
    idx_t idx = (idx_t)val;
    if (idx == 0) return;               /* we never store 0 */
    ensure_used_capacity(idx);
    used_bits[idx >> 6] |= 1ULL << (idx & 63);
}

/* Return true iff `val` has already appeared in the sequence. */
static bool used_has(seq_t val)
{
    idx_t idx = (idx_t)val;
    if (idx == 0) return false;         /* 0 cannot appear */
    if ((size_t)idx >= used_bits_cap) return false;
    return ((used_bits[idx >> 6] >> (idx & 63)) & 1ULL) != 0;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    /* Initialise on the first call. */
    if (max_n == 0) {
        ensure_values_capacity(2);
        values[1] = (seq_t)1;
        values[2] = (seq_t)3;
        used_set(values[1]);
        used_set(values[2]);
        max_n = 2;
    }

    /* Return the value if it has already been computed. */
    if (n <= max_n) return values[n];

    /* Compute missing terms up to `n`. */
    for (idx_t curr = max_n + 1; curr <= n; ++curr) {
        seq_t prev = values[curr - 1];
        seq_t candidate = prev - (seq_t)1;
        bool can_subtract = candidate > (seq_t)0 && !used_has(candidate);
        seq_t cur = can_subtract ? candidate : prev + (seq_t)curr;

        ensure_values_capacity(curr);
        values[curr] = cur;
        used_set(cur);
        max_n = curr;
    }

    return values[n];
}
