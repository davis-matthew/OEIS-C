/*
 * A043756  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 2 mod 9.
 * https://oeis.org/A043756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043756: all positive integers n such that the number of binary runs
 * (maximal consecutive identical bits) of n is congruent to 2 modulo 9. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* count the number of binary runs in x */
static int bin_runs(seq_t x)
{
    int runs = 0;
    seq_t prev = (seq_t)(-1);   /* value that differs from 0 or 1 */
    while (x) {
        seq_t bit = x & 1;
        if (bit != prev) {
            runs++;
            prev = bit;
        }
        x >>= 1;
    }
    return runs;
}

/* static cache for computed terms */
static seq_t *seq   = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
/* next integer to test */
static idx_t next_candidate = 1;

/* return the n-th term of the sequence (a(1)=first term) */
seq_t a(idx_t n)
{
    if (seq == NULL) {
        seq_cap = 16;
        seq_len = 0;
        seq = mem_new(seq_t, seq_cap);
    }

    while (seq_len < (size_t)n) {
        idx_t val64 = next_candidate;   /* integer to test */
        next_candidate++;               /* advance for next call */
        seq_t val = (seq_t)val64;
        if (bin_runs(val) % 9 == 2) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = val;
        }
    }
    return seq[n - 1];
}
