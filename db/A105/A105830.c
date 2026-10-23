/*
 * A105830  For n>2, a(n) not appeared previously is such that
 *          a(n-1)^2+4*a(n-2)*a(n) is a minimal square, a(1)=1, a(2)=5.
 * https://oeis.org/A105830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105830: a(1)=1, a(2)=5; for n>2, a(n) is the smallest
   positive integer not yet appeared such that
   a(n‑1)^2 + 4*a(n‑2)*a(n) is a perfect square. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t int_sqrt(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

static bool is_square(seq_t x)
{
    seq_t r = int_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    /* static state: computed terms */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0, seq_cap = 0;

    /* initialise with a(1)=1, a(2)=5 on first call */
    if (seq_len == 0) {
        seq_cap = 2;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)5;
        seq_len = 2;
    }

    /* extend sequence up to requested index */
    while (seq_len < n) {
        seq_t a_prev      = seq[seq_len - 1];
        seq_t a_prevprev  = seq[seq_len - 2];
        seq_t candidate   = (seq_t)1;

        for (;;) {
            /* skip already used values */
            bool used = false;
            for (idx_t i = 0; i < seq_len; ++i) {
                if (seq[i] == candidate) {
                    used = true;
                    break;
                }
            }
            if (used) {
                ++candidate;
                continue;
            }

            /* test condition a_prev^2 + 4*a_prevprev*candidate is a square */
            seq_t term = a_prev * a_prev + ((seq_t)4 * a_prevprev * candidate);
            if (is_square(term)) {
                /* ensure capacity */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 32;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = candidate;
                break;   /* next term appended */
            }
            ++candidate;
        }
    }

    return seq[n - 1];
}
