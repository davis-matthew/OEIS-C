/*
 * A171087  A "run" means a maximal string of numbers that includes exactly
 *          one even number; this sequence has the property that the sequence
 *          of run lengths is the sequence itself and all natural numbers
 *          appear exactly once.
 * https://oeis.org/A171087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171087 – Run‑length construction (built iteratively). */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *seq = NULL;   /* array of generated terms */
static size_t seq_cap = 0;  /* current capacity of seq */
static size_t seq_len = 0;  /* number of terms stored */

static idx_t next_run = 1;       /* next run index (1‑based) */
static idx_t next_odd = 1;       /* smallest unused odd number */

/* enlarge the dynamic array so it can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) exit(EXIT_FAILURE);
        seq = new_seq;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 – defensive */

    /* generate terms until we reach the requested index */
    while (seq_len < (size_t)n) {
        /* ---------- run length of current run ---------- */
        seq_t run_len;
        if (next_run == 1) {
            run_len = (seq_t)2;  /* a(1) = 2 */
        } else {
            run_len = seq[(size_t)next_run - 1];  /* a(next_run) */
        }

        /* ---------- append the even number ---------- */
        seq_t even = (seq_t)2 * (seq_t)next_run;
        ensure_capacity(seq_len + 1);
        seq[seq_len++] = even;

        /* ---------- append required odd numbers ---------- */
        idx_t need_odds = (idx_t)(run_len - (seq_t)1);
        for (idx_t i = 0; i < need_odds; ++i) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)next_odd;
            next_odd += 2;            /* next unused odd */
        }

        /* advance to the next run */
        ++next_run;
    }

    return seq[(size_t)n - 1];
}
