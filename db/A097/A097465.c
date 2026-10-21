/*
 * A097465  a(1) = 1; for n>1, a(n) = smallest positive integer which is not
 *          among earlier terms of sequence, is coprime to a(n-1) and is not
 *          equal to a(n-1) +- 1.
 * https://oeis.org/A097465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS recurrence:
   a(1) = 1;
   for n > 1,
   a(n) = smallest positive integer not already used,
          coprime to a(n-1), and not equal to a(n-1) ± 1. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static memoization state */
    static seq_t *seq = NULL;        /* actual sequence values */
    static size_t seq_cap = 0;       /* allocated capacity for seq[] */
    static size_t seq_len = 0;       /* number of values currently stored */

    static seq_t last = 0;           /* a(k-1) for the last computed k */

    static uint8_t *used = NULL;     /* flag array: used[x] == 1 means x already used */
    static size_t used_cap = 0;      /* current allocated size of used[] */
    static idx_t used_max = 0;       /* largest index ever accessed in used[] */

    /* ---------- initialisation on first call ---------- */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t*)calloc(seq_cap, sizeof(seq_t)); /* cast to silence C++ warnings */
        seq[0] = (seq_t)1;                            /* a(1) = 1  */
        seq_len = 1;
        last   = (seq_t)1;

        used_cap = 65536;                             /* start with some room */
        used = (uint8_t*)calloc(used_cap, sizeof(uint8_t));
        used[1] = 1;  /* mark 1 as used   */
        used_max = 1;
    }

    /* ---------- return already known value ---------- */
    if (n <= seq_len) return seq[n-1];

    /* ---------- extend sequence up to n ---------- */
    for (idx_t k = seq_len + 1; k <= n; ++k) {
        seq_t candidate = 1;

        /* search for the smallest valid candidate */
        for (;;) {
            idx_t idx = (idx_t)candidate;

            /* grow the used flag array if necessary */
            if (idx >= used_cap) {
                size_t new_cap = used_cap * 2;
                while (idx >= new_cap)
                    new_cap *= 2;
                uint8_t *tmp = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
                memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
                used = tmp;
                used_cap = new_cap;
            }

            if (!used[idx] &&
                candidate != last + (seq_t)1 &&
                candidate != last - (seq_t)1 &&
                gcd(candidate, last) == 1) {
                break;          /* found a valid candidate */
            }
            ++candidate;
        }

        /* mark the chosen candidate as used */
        idx_t used_idx = (idx_t)candidate;
        used[used_idx] = 1;
        if (used_idx > used_max) used_max = used_idx;

        /* grow seq array if needed */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }

        /* store the new value and update state */
        seq[seq_len++] = candidate;
        last = candidate;
    }

    return seq[n-1];
}
