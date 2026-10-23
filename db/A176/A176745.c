/*
 * A176745  Zero followed by powers of 2 (A131577) and the integers that
 *          cannot be represented as a sum of two earlier terms in the
 *          sequence.
 * https://oeis.org/A176745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176745: smallest integer > previous term which is a power of 2
   or cannot be represented as a sum of two earlier terms. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_power_of_two.h"

/* dynamic array of terms */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
/* bitset that records all sums of two earlier terms */
static uint64_t *sum_bits = NULL;
static size_t sum_bits_len = 0;   /* in bits */

/* ensure seq has capacity for at least needed elements */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq = new_arr;
    seq_cap = new_cap;
}

/* ensure sum_bits has storage for at least bits_needed bits */
static void ensure_sum_bits(size_t bits_needed)
{
    if (bits_needed <= sum_bits_len) return;
    size_t new_len = sum_bits_len ? sum_bits_len << 1 : 64;
    while (new_len < bits_needed) new_len <<= 1;
    size_t new_words = (new_len + 63) >> 6;
    size_t old_words  = (sum_bits_len + 63) >> 6;
    uint64_t *new_arr = (uint64_t *)realloc(sum_bits, new_words * sizeof(uint64_t));
    if (!new_arr) abort();
    if (new_words > old_words)
        memset(new_arr + old_words, 0, (new_words - old_words) * sizeof(uint64_t));
    sum_bits   = new_arr;
    sum_bits_len = new_len;
}

/* test whether a bit for value x is set in the bitset */
static inline bool sum_bit_set(seq_t x)
{
    if (x > (seq_t)UINT64_MAX) return false;
    size_t idx = (size_t)x;
    if (idx >= sum_bits_len) return false;
    return ((sum_bits[idx >> 6] >> (idx & 63)) & 1ULL) != 0;
}

/* binary search for value in sorted array [0, len) */
static bool seq_contains(const seq_t *arr, size_t len, seq_t v)
{
    size_t left = 0, right = len;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        seq_t midv = arr[mid];
        if (midv < v)
            left = mid + 1;
        else if (midv > v)
            right = mid;
        else
            return true;
    }
    return false;
}

/* true if v can be expressed as the sum of two earlier terms */
static bool can_be_sum(seq_t v)
{
    /* fast path using bitset when v fits in 64 bits */
    if (v <= (seq_t)UINT64_MAX && sum_bit_set(v)) return true;

    /* fallback: check by searching */
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t a = seq[i];
        /* a < v always holds because all earlier terms are < v */
        seq_t b = v - a;
        if (seq_contains(seq, seq_len, b)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0) {
        ensure_seq_cap(64);
        seq[0] = 0;
        seq_len = 1;
    }

    /* generate terms up to index n */
    while (seq_len <= (size_t)n) {
        seq_t candidate = seq[seq_len - 1] + 1;
        for (;;) {
            bool ok = false;
            if (candidate == 0)
                ok = true;
            else if (is_power_of_two(candidate))
                ok = true;
            else if (!can_be_sum(candidate))
                ok = true;

            if (ok) break;
            ++candidate;
        }

        /* store the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = candidate;

        /* update sums: candidate with all earlier terms, including itself */
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t sum = candidate + seq[i];
            if (sum <= (seq_t)UINT64_MAX) {
                size_t bitpos = (size_t)sum;
                ensure_sum_bits(bitpos + 1);
                sum_bits[bitpos >> 6] |= 1ULL << (bitpos & 63);
            }
        }

        ++seq_len;
    }

    return seq[(size_t)n];
}
