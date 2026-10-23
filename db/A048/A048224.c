/*
 * A048224  a(n)=least number m such that m-a(n-1)<>a(j)-a(k) for all j,k less
 *          than m; a(1)=1, a(2)=4.
 * https://oeis.org/A048224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4;
   for n>2: a(n)=a(n-1)+s, where s is the smallest positive integer
   not equal to any difference a(j)-a(k) for 1≤j,k<n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void init_diff_bits(size_t max_needed);
static void resize_diff_bits(size_t new_needed);
static void set_diff_bit(idx_t val);
static bool has_diff_bit(idx_t val);

static uint64_t *diff_bits = NULL;            /* bit array for differences */
static size_t diff_bits_cap = 0;              /* number of 64‑bit words */
static idx_t max_diff = 0;                    /* maximum diff stored */

static void init_diff_bits(size_t max_needed) {
    diff_bits_cap = (max_needed + 63) / 64;
    diff_bits = (uint64_t *)calloc(diff_bits_cap, sizeof(uint64_t));
}

static void resize_diff_bits(size_t new_needed) {
    size_t new_cap = (new_needed + 63) / 64;
    if (new_cap <= diff_bits_cap) return;
    diff_bits = (uint64_t *)realloc(diff_bits, new_cap * sizeof(uint64_t));
    /* initialise new words to zero */
    for (size_t w = diff_bits_cap; w < new_cap; ++w) diff_bits[w] = 0;
    diff_bits_cap = new_cap;
}

static void set_diff_bit(idx_t val) {
    idx_t word = val >> 6;          /* divide by 64 */
    diff_bits[word] |= (uint64_t)1 << (val & 63);
}

static bool has_diff_bit(idx_t val) {
    idx_t word = val >> 6;
    if (word >= diff_bits_cap) return false;
    return (diff_bits[word] >> (val & 63)) & 1;
}

/*------------------------ sequence ---------------------------------*/

static seq_t *seq = NULL;                 /* stored terms */
static idx_t seq_cap = 0;                 /* capacity of seq array */
static idx_t seq_len = 0;                 /* number of terms stored */
static idx_t next_step = 1;               /* candidate step for next term */

seq_t a(idx_t n)
{
    /* initialise once */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* allocation failure */

        /* first two terms */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        seq_len = 2;

        /* differences of the first two terms: 3 */
        init_diff_bits(3);                     /* allocate bitmap */
        set_diff_bit((idx_t)((seq_t)3));
        max_diff = 3;

        next_step = 1;
    }

    /* grow sequence up to n */
    while (seq_len < n) {
        seq_t const prev = seq[seq_len - 1];
        idx_t s = next_step;

        /* find smallest s not present as a positive difference */
        while (has_diff_bit(s)) ++s;

        seq_t const new_val = prev + (seq_t)s;

        /* append new term */
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) return (seq_t)0;   /* allocation failure */
        }
        seq[seq_len] = new_val;
        ++seq_len;

        /* add all new positive differences */
        for (idx_t i = 0; i < seq_len - 1; ++i) {
            seq_t diff = new_val - seq[i];     /* diff > 0 */
            idx_t diff_idx = (idx_t)diff;      /* value is small (≤ new_val) */
            if (diff_idx > max_diff) {
                resize_diff_bits(diff_idx);
                max_diff = diff_idx;
            }
            set_diff_bit(diff_idx);
        }

        /* step s is now present; next candidate starts from s */
        next_step = s;
    }

    return seq[n - 1];
}
