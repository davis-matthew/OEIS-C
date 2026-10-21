/*
 * A091784  Numbers n with digits in nondecreasing order such that sum of the
 *          reciprocal of digits is an integer.
 * https://oeis.org/A091784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th smallest positive integer whose decimal digits
 *            are in * nondecreasing order and for which the sum of the
 *            reciprocals of those digits * is an integer. The algorithm
 *            enumerates all nondecreasing digit strings * in increasing
 *            numeric order and keeps a running sum modulo 2520 (the lcm...
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 637, u64 n <= 9072,
 *            u128 n <= 9072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- helpers for dynamic array of seq_t ----------------------------------- */

static seq_t *seqs = NULL;      /* array of found terms */
static size_t seqs_len = 0;     /* current number of terms */
static size_t seqs_cap = 0;     /* allocated capacity */

/* ensure capacity for at least need elements */
static void seqs_grow(size_t need)
{
    if (need <= seqs_cap) return;
    size_t new_cap = seqs_cap ? seqs_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seqs = (seq_t *)realloc(seqs, new_cap * sizeof(seq_t));
    seqs_cap = new_cap;
}

/* store a new term */
static void seqs_push(seq_t val)
{
    seqs_grow(seqs_len + 1);
    seqs[seqs_len++] = val;
}

/* ---- enumeration of nondecreasing digit numbers -------------------------- */

static const int ADD_MOD[10] = {0,      /* unused index 0 */
                                0,     /* 2520/1  ≡ 0  (mod 2520) */
                                1260,  /* 2520/2  */
                                840,   /* 2520/3  */
                                630,   /* 2520/4  */
                                504,   /* 2520/5  */
                                420,   /* 2520/6  */
                                360,   /* 2520/7  */
                                315,   /* 2520/8  */
                                280};  /* 2520/9  */

/* depth–first search that generates all numbers of a given length.
 * Parameters:
 *   len      – desired total length
 *   pos      – current position (0 … len-1)
 *   last_d   – minimal digit allowed at this position (to keep nondecreasing)
 *   value    – value built so far (seq_t)
 *   mod      – current sum of 2520/digits modulo 2520 */
static void dfs_generate(int len, int pos, int last_d, seq_t value, int mod)
{
    if (pos == len) {
        if (mod == 0) seqs_push(value);
        return;
    }
    for (int d = last_d; d <= 9; ++d) {
        seq_t new_value = value * 10 + (seq_t)d;
        int new_mod = (mod + ADD_MOD[d]) % 2520;
        dfs_generate(len, pos + 1, d, new_value, new_mod);
    }
}

/* global variable to remember the next length to generate
 * once a new length has been processed we increment it. */
static int next_len_to_generate = 1;

/* ---- sequence entry function ---------------------------------------------- */

seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (seqs_len < n) {
        dfs_generate(next_len_to_generate, 0, 1, (seq_t)0, 0);
        ++next_len_to_generate;
    }
    return seqs[n - 1];
}
