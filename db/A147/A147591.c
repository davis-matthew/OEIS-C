/*
 * A147591  RootMeanSquare(digits of n) is an integer.
 * https://oeis.org/A147591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th positive integer whose root‑mean‑square of
 *            decimal digits is an integer
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static inline bool is_rms_integer(seq_t n)
{
    seq_t t = n;
    seq_t sum = (seq_t)0;
    size_t len = 0;
    while (t > (seq_t)0) {
        uint64_t d = (uint64_t)(t % (seq_t)10);
        sum += (seq_t)(d * d);
        t   /= (seq_t)10;
        len++;
    }
    if (len == 0) return false;
    seq_t len_seq = (seq_t)len;
    if (sum % len_seq != (seq_t)0) return false;
    seq_t avg = sum / len_seq;
    seq_t k   = floor_sqrt(avg);
    return k * k == avg;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safe guard */
    while (seq_len < (size_t)n) {
        seq_t candidate;
        if (seq_len == 0) candidate = (seq_t)1;
        else             candidate = seq[seq_len - 1] + (seq_t)1;

        while (!is_rms_integer(candidate))
            candidate = candidate + (seq_t)1;

        ensure_capacity(seq_len + 1);
        seq[seq_len++] = candidate;
    }
    return seq[n - 1];
}
