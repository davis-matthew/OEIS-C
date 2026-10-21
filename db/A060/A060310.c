/*
 * A060310  a(1)=1, a(2)=3; then append digits of a(n-1)*a(n).
 * https://oeis.org/A060310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=3; then for each consecutive pair (a(k),a(k+1))
 *            append the decimal digits of their product to the sequence. The
 *            n‑th term of the sequence is a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build; no definitions are made here. */

static seq_t *seq = NULL;          /* 1‑based array of digits (0…9) */
static idx_t seq_cap = 0;          /* current capacity of seq */
static idx_t seq_len = 0;          /* number of terms stored so far (>= seq_len) */
static idx_t prod_start = 1;       /* start index of the next product pair */

/* Ensure seq has enough capacity for at least “needed” indices (1‑based). */
static void ensure_capacity(idx_t needed)
{
    if (seq_cap <= needed) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Main sequence generator.  Handles any call order and caches previously
   computed terms. */
seq_t a(idx_t n)
{
    /* Once: initialise on first call */
    if (seq_len == 0) {
        ensure_capacity(16);
        seq[1] = 1;
        seq[2] = 3;
        seq_len = 2;
        prod_start = 1;
    }

    /* Extend the sequence until we have reached the requested index. */
    while (seq_len < n) {
        seq_t prod = seq[prod_start] * seq[prod_start + 1];
        if (prod < 10) {
            /* Single digit */
            ensure_capacity(seq_len + 1);
            seq[++seq_len] = prod;
        } else {
            /* Two digits: high then low */
            seq_t high = prod / 10;
            seq_t low  = prod % 10;
            ensure_capacity(seq_len + 2);
            seq[++seq_len] = high;
            seq[++seq_len] = low;
        }
        prod_start++;                /* advance to the next pair */
    }

    return seq[n];
}
