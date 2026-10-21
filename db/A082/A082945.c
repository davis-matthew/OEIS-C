/*
 * A082945  Numbers n which in decimal have the form imj, where m is the
 *          middle digit, with property that j is the reversal of i, and i =
 *          m*j.
 * https://oeis.org/A082945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   rev_uint64, seq_compare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation: generate all numbers of the form imj where
    j is the reversal of i, i = m*j, i does not end in 0, and m is a
    single non‑zero digit (m = 1 for palindromic i or m = 4,9 otherwise). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/rev_uint64.h"
#include "../utils/seq_compare.h"

/* Forward declaration of external types.
   They are defined by the build system; do **not** redefine them here.
   typedef unsigned long long idx_t;   // example
   typedef unsigned long long seq_t;   // example */

/* Static storage for the sequence values. */
static seq_t *seq_vals   = NULL;
static size_t seq_cap    = 0;
static size_t seq_cnt    = 0;
static int    seq_ready  = 0;
static seq_t  pow10[20];

/* Generate the sequence once. */
static void prepare_sequence(void)
{
    if (seq_ready) return;

    /* Pre‑compute powers of ten.  pow10[d] = 10^d. */
    pow10[0] = (seq_t)1;
    for (size_t i = 1; i < 20; ++i)
        pow10[i] = pow10[i-1] * (seq_t)10;

    /* Initial capacity – plenty for the first 10⁴ terms. */
    seq_cap = 20000;
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_vals) exit(1);
    seq_cnt = 0;

    /* Upper bound for i: enough to produce >10⁴ terms. */
    const uint64_t I_LIMIT = 10000000ULL;   /* 10 million */

    uint64_t next10   = 10;
    int      digits_i = 1;

    for (uint64_t i = 1; i <= I_LIMIT; ++i) {
        if (i == next10) {
            ++digits_i;
            next10 *= 10;
        }

        /* i must not end in 0. */
        if ((i % 10) == 0) continue;

        uint64_t j = rev_uint64(i);
        if (j == 0) continue;

        if (i % j != 0) continue;   /* i must be divisible by j */

        uint64_t m = i / j;
        if ((m != 1) && (m != 4) && (m != 9)) continue;

        /* Construct imj: i * 10^(d+1) + m * 10^d + j */
        seq_t n = ((seq_t)i) * pow10[digits_i + 1]
                 + ((seq_t)m) * pow10[digits_i]
                 + ((seq_t)j);

        /* Store it, growing the array if needed. */
        if (seq_cnt == seq_cap) {
            seq_cap <<= 1;
            seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            if (!seq_vals) exit(1);
        }
        seq_vals[seq_cnt++] = n;
    }

    /* Sort the terms so that a(1) is the smallest, etc. */
    qsort(seq_vals, seq_cnt, sizeof(seq_t), seq_compare);

    seq_ready = 1;
}

/* Required external function.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    prepare_sequence();
    /* No bounds checking – the test harness guarantees n is valid. */
    return seq_vals[n - 1];
}
