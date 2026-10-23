/*
 * A118247  a(0)=0. Concatenate onto the end of the sequence (from left to
 *          right) the integer m_n converted into binary (with the most
 *          significant digit on the left), where m_n is the smallest integer
 *          > A118248(n-1) and whose binary representation does not occur
 *          anywhere earlier in the sequence (when the con...
 * https://oeis.org/A118247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 338, u16 n <= 9534, u32 n <= 9534, u64 n <= 9534,
 *            u128 n <= 9534, big any n
 * Helpers:   bitlen_seq_t, pattern_in_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118247: a(n) = the n-th bit of the binary concatenation of the
   sequence A118248.  The function builds the bitstream lazily,
   maintaining the smallest candidate m that has not yet occurred
   as a contiguous block, appends its binary representation, and
   continues until it can answer the requested index. */

/* Required utilities:
   ../utils/bitlen_seq_t.h  (bitlen_seq_t returns the length in bits of a seq_t value) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/pattern_in_sequence.h"

/* static state for lazy construction */
static uint8_t *seq_bits = NULL;   /* bitstream, each element 0 or 1 */
static size_t  seq_len  = 0;       /* number of bits stored so far   */
static size_t  seq_cap  = 0;       /* allocated capacity (bytes)     */
static seq_t   last_m   = 0;       /* last appended m value          */
static bool    initialized = false;   /* whether state has been set up */

/* Ensure that seq_bits has capacity for at least 'need' bits. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1024;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(seq_bits, new_cap);
    if (!tmp) abort();
    seq_bits = tmp;
    seq_cap  = new_cap;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    /* Lazy initialization on first call. */
    if (!initialized) {
        seq_cap  = 1024;
        seq_bits = (uint8_t *)malloc(seq_cap);
        if (!seq_bits) abort();
        seq_bits[0] = 0;     /* m_0 = 0 contributes bit '0' */
        seq_len    = 1;
        last_m     = 0;
        initialized = true;
    }

    /* Grow the sequence until we have bit n. */
    while (seq_len <= (size_t)n) {
        seq_t candidate = last_m + 1;

        /* Find smallest candidate whose binary pattern is absent. */
        for (;;) {
            size_t pat_len = bitlen_seq_t(candidate);
            uint8_t *pat = (uint8_t *)malloc(pat_len);
            if (!pat) abort();
            for (size_t i = 0; i < pat_len; ++i) {
                pat[i] = (uint8_t)(((candidate >> (pat_len - 1 - i)) & 1) != 0);
            }
            bool present = pattern_in_sequence(seq_bits, seq_len, pat, pat_len);
            free(pat);
            if (!present) break;          /* candidate chosen */
            ++candidate;                  /* try next integer */
        }

        /* Append bits of chosen candidate to the stream. */
        size_t pat_len = bitlen_seq_t(candidate);
        uint8_t *pat = (uint8_t *)malloc(pat_len);
        if (!pat) abort();
        for (size_t i = 0; i < pat_len; ++i) {
            pat[i] = (uint8_t)(((candidate >> (pat_len - 1 - i)) & 1) != 0);
        }
        ensure_capacity(seq_len + pat_len);
        for (size_t i = 0; i < pat_len; ++i) seq_bits[seq_len++] = pat[i];
        free(pat);

        last_m = candidate;    /* update for next iteration */
    }

    return (seq_t)seq_bits[n];
}
