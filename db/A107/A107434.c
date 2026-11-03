/*
 * A107434  Smallest available integer allowing a digit of a(n) and a digit of
 *          a(n+1) to add up to 9.
 * https://oeis.org/A107434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107434:
   a(0) = 0.
   For n > 0, a(n) is the smallest integer not yet used whose
   decimal representation contains a digit d such that there is a
   digit e in a(n‑1) with d + e == 9. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* return true if integer x contains a digit that is allowed by
   the 10‑bit mask 'mask' (bit i is set if digit i is allowed) */
static bool has_allowed_digit(seq_t x, uint16_t mask)
{
    if (x == 0) {
        /* digit 0 is present */
        return (mask & 1u) != 0;
    }
    while (x > 0) {
        int d = (int)(x % 10);
        if (mask & (1u << d))
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0, seq_cap = 0;
    static bool *used = NULL;
    static idx_t used_cap = 0;

    /* initialise storage on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = 0;
        seq_len = 1;            /* a(0) is known */
    }
    if (used_cap == 0) {
        used_cap = 32;
        used = (bool *)calloc(used_cap, sizeof(bool));
        used[0] = true;             /* 0 has already been used */
    }

    /* generate terms up to index n */
    while (seq_len <= n) {
        /* build mask of allowed digits from the previous term */
        uint16_t mask = 0;
        seq_t last = seq[seq_len - 1];
        if (last == 0) {
            mask |= 1u << 9;          /* 0's complement is 9 */
        } else {
            seq_t tmp = last;
            while (tmp > 0) {
                int d = (int)(tmp % 10);   /* digit of last term */
                mask |= 1u << (9 - d);     /* its complement */
                tmp /= 10;
            }
        }

        /* find the smallest unused integer satisfying the mask */
        idx_t x = 0;
        seq_t cand = (seq_t)0;
        for (;;) {
            if (x >= used_cap) {          /* grow used bitmap if needed */
                size_t newcap = used_cap * 2;
                if (newcap == 0) newcap = 1;
                bool *tmp = (bool *)realloc(used, newcap * sizeof(bool));
                for (size_t i = used_cap; i < newcap; ++i)
                    tmp[i] = false;
                used = tmp;
                used_cap = newcap;
            }
            if (!used[x] && has_allowed_digit((seq_t)x, mask)) {
                cand = (seq_t)x;
                used[x] = true;
                break;
            }
            ++x;
        }

        /* store the new term */
        if (seq_len >= seq_cap) {
            size_t newcap = seq_cap * 2;
            seq_t *tmp = mem_resize(seq_t, seq, newcap);
            seq = tmp;
            seq_cap = newcap;
        }
        seq[seq_len] = cand;
        ++seq_len;
    }

    return seq[n];
}
