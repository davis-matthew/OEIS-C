/*
 * A107778  a(1)=7, a(n) = smallest integer not previously used which contains
 *          a digit from a(n-1).
 * https://oeis.org/A107778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_mask, ensure_seq_capacity, ensure_used_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=7; for n>1, a(n)= smallest unused integer containing a digit from a(n-1). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_mask.h"
#include "../utils/ensure_seq_capacity.h"
#include "../utils/ensure_used_capacity.h"

static inline uint16_t digit_mask_seq_t(seq_t x)
{
    return (x == 0) ? 1 : digit_mask(x);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined but harmless */

    /* Static state shared between calls */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;

    /* One‑time initialisation */
    if (!seq) {
        ensure_seq_capacity(1, &seq, &seq_cap);
        seq_len = 0;
    }
    if (!used) {
        ensure_used_capacity(1, &used, &used_cap);
    }

    /* Ensure the first term (a(1)=7) is present */
    if (seq_len == 0) {
        seq[0] = 7;
        size_t idx = 7;
        ensure_used_capacity(idx + 1, &used, &used_cap);
        used[idx] = true;
        seq_len = 1;
    }

    /* Generate terms up to requested index n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        uint16_t prev_mask = digit_mask_seq_t(prev);

        /* Find smallest unused candidate sharing a digit with prev */
        for (seq_t cand = 0;; ++cand) {
            idx_t cand_idx = (idx_t)cand;
            if (cand_idx >= used_cap) {
                ensure_used_capacity(cand_idx + 1, &used, &used_cap);
            }
            if (!used[cand_idx]) {
                uint16_t cand_mask = digit_mask_seq_t(cand);
                if (cand_mask & prev_mask) {
                    /* Store the new term */
                    if (seq_len >= seq_cap) {
                        ensure_seq_capacity(seq_len + 1, &seq, &seq_cap);
                    }
                    seq[seq_len] = cand;
                    used[cand_idx] = true;
                    ++seq_len;
                    break; /* next term generated */
                }
            }
        }
    }

    return seq[n - 1];
}
