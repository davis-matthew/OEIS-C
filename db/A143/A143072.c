/*
 * A143072  A positive integer n is included if both the number of 0's and the
 *          number of 1's in the binary representation of n are powers of 2
 *          (including being possibly 1).
 * https://oeis.org/A143072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 7046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A143072
   A positive integer n is included if both the number of 0's and the
   number of 1's in the binary representation of n are powers of 2.
   This function generates the sequence on demand, caching the terms
   that have already been found. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Cached sequence */
    static seq_t *seq = NULL;       /* array of found terms */
    static size_t seq_cap = 0;      /* allocated capacity */
    static size_t seq_len = 0;      /* number of terms stored */
    static seq_t next_search = 2;   /* next integer to test */

    /* initialise storage */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n */
    while (seq_len < n) {
        /* Find the next valid integer */
        for (unsigned long long v = (unsigned long long)next_search; ; ++v) {
            seq_t cand = (seq_t)v;

            /* Popcount and length of binary representation */
            unsigned long long pop = __builtin_popcountll(v);
            unsigned long long bitlen = 0, tmp = v;
            while (tmp) { bitlen++; tmp >>= 1; }
            unsigned long long zeros = bitlen - pop;

            /* Check the power‑of‑two conditions */
            if (pop > 0 && zeros > 0 &&
                is_power_of_two((seq_t)pop) &&
                is_power_of_two((seq_t)zeros)) {

                /* Grow storage if necessary */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = cand;
                next_search = (seq_t)(v + 1);
                break;                      /* next term found */
            }
        }
    }

    /* Return the requested term */
    return seq[n - 1];
}
