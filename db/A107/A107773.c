/*
 * A107773  a(1)=2, a(n) = smallest integer not previously used which contains
 *          a digit from a(n-1).
 * https://oeis.org/A107773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   has_common_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=2, a(n)=smallest unused integer containing a digit from a(n‑1).
 *  The sequence is generated incrementally and memoised; no hard‑coded values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_common_digit.h"

/* The types idx_t and seq_t are supplied by the build environment; do not redeclare them. */

/* Bit‑set for marking which integers have already appeared in the sequence. */
static uint8_t *used_bits = NULL;      /* array of bytes, one bit per integer */
static idx_t   used_cap  = 0;           /* number of bits currently allocated */

/* Stored sequence values (1‑based). */
static seq_t *seq      = NULL;          /* array of sequence terms */
static idx_t  seq_len  = 0;             /* number of terms computed so far */

/* Ensure the bit‑set can record the integer v. */
static void ensure_used_cap(idx_t v)
{
    if (v < used_cap)
        return;                         /* already large enough */

    /* Grow capacity to the next power of two ≥ v+1. */
    idx_t new_cap = used_cap ? used_cap << 1 : 1024;
    while (new_cap <= v)
        new_cap <<= 1;

    size_t new_bytes = (new_cap + 7) >> 3;
    size_t old_bytes = (used_cap + 7) >> 3;

    if (!used_bits) {
        used_bits = (uint8_t *)calloc(new_bytes, 1);
        if (!used_bits) abort();
        used_cap = new_cap;
        return;
    }

    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
    if (!tmp) abort();
    if (new_bytes > old_bytes)
        memset(tmp + old_bytes, 0, new_bytes - old_bytes);

    used_bits = tmp;
    used_cap  = new_cap;
}

/* Mark integer v as used. */
static void set_used(idx_t v)
{
    ensure_used_cap(v);
    used_bits[v >> 3] |= 1u << (v & 7);
}

/* Test whether integer v has already appeared. */
static inline bool is_used(idx_t v)
{
    if (v >= used_cap) return false;
    return (used_bits[v >> 3] >> (v & 7)) & 1u;
}

/* Return a bitmask of decimal digits present in x (bits 0‑9). */
static unsigned int digit_mask(seq_t x)
{
    if (x == 0) return 1u;          /* only digit 0 */
    unsigned int mask = 0;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

/* Compute terms up to index n (1‑based). */
static void ensure_sequence_upto(idx_t n)
{
    if (n <= seq_len) return;       /* already computed */

    /* Allocate or enlarge the sequence array. */
    if (!seq) {
        seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!seq) abort();
    } else if (seq_len + 1 < n + 1) {
        seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
        if (!seq) abort();
    }

    /* Generate missing terms. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t next;
        if (i == 1) {
            next = (seq_t)2;
        } else {
            unsigned int maskPrev = digit_mask(seq[i - 1]);

            /* Search for the smallest unused integer containing a digit from a(i‑1). */
            for (idx_t cand = 0;; ++cand) {
                if (is_used(cand)) continue;
                if (has_common_digit(cand, maskPrev)) {
                    next = (seq_t)cand;
                    break;
                }
            }
        }
        seq[i] = next;
        set_used((idx_t)next);
    }

    seq_len = n;
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    ensure_sequence_upto(n);
    return seq[n];
}
